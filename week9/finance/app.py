# Web app that allows users to trade and sell stocks
# personal touch - restrict user from registering simple password
#                  - allow user to change password
#                  - allow user to load (add cash) to their acount

import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Included by the student
from helpers import is_Owned
from datetime import datetime
from string import ascii_lowercase, ascii_uppercase, digits, punctuation
import re

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # Fetch buys of the user from the database
    infos = db.execute(
        "SELECT users.username, stock_name, stock_symbol, SUM(number_of_shares) AS number_of_shares, cash FROM users JOIN buys ON users.username = buys.username WHERE id = ? GROUP BY stock_symbol", session["user_id"])
    # print(infos)

    # If the user don't have a share of any stock
    if len(infos) == 0:
        user = db.execute("SELECT username, cash FROM users WHERE id = ?", session["user_id"])
        return render_template("index.html", infos=infos, cash=usd(user[0]["cash"]), username=user[0]["username"])

    # Loop through infos and change the number of shares to be correct
    total = infos[0]["cash"]
    for row in infos:
        sold_stock = db.execute(
            "SELECT username, stock_name, stock_symbol, SUM(number_of_shares) AS number_of_shares FROM sells WHERE username = ? AND stock_symbol = ?", row["username"], row["stock_symbol"])

        if sold_stock[0]["username"] is not None:

            # Subtract the sells to buys to reflect the correct number of shares owned
            if len(sold_stock) > 0:
                row["number_of_shares"] -= sold_stock[0]["number_of_shares"]

        # Update variables
        if row["number_of_shares"] > 0:
            stock = lookup(row["stock_symbol"])
            row["live_price"] = usd(stock["price"])
            row["live_total"] = usd(stock["price"] * row["number_of_shares"])
            total += (stock["price"] * row["number_of_shares"])

    # # Remove stocks that is no longer owned
    infos = list(filter(is_Owned, infos))

    # If the user don't have a share of any stock
    if len(infos) == 0:
        user = db.execute("SELECT username, cash FROM users WHERE id = ?", session["user_id"])
        return render_template("index.html", infos=infos, cash=usd(user[0]["cash"]), username=user[0]["username"])

    # Load the homepage along with necessary variables
    return render_template("index.html", username=infos[0]["username"], infos=infos, cash=usd(infos[0]["cash"]), total=usd(total))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        stock_info = lookup(request.form.get("symbol"))
        no_shares = (request.form.get("shares"))

        # Check for errors (empty stocks or it do not exists)
        if not stock_info:
            return apology("stock do not exists")

        # Check for errors (no_shares is non positive integers)
        if not no_shares.isnumeric():
            return apology("number of shares must be positive")
        no_shares = int(no_shares)
        if no_shares <= 0:
            return apology("number of shares must be positive")

        # Check for errors (user lacks money)
        user_id = session["user_id"]
        user_balance = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]
        total_price = stock_info["price"] * no_shares

        if user_balance < total_price:
            return apology("Your cash is insufficient to buy the shares")

        # Update the database about the transaction
        username = db.execute("SELECT username FROM users WHERE id = ?", user_id)[0]["username"]
        db.execute("INSERT INTO buys VALUES (?, ?, ?, ?, ?, ?, ?, 'Buy')", datetime.now(), username,
                   stock_info["name"], stock_info["symbol"], stock_info["price"], no_shares, total_price)
        db.execute("UPDATE users SET cash = ? WHERE id = ?", user_balance - total_price, user_id)

        # Redirect to receipt
        return render_template("bought.html", symbol=stock_info["symbol"], price=usd(stock_info["price"]), no_shares=no_shares, total=usd(total_price), balance=usd(user_balance - total_price))

    # GET via clicking links
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    # Get all buys and sells of the user from database
    buy_transactions = db.execute(
        "SELECT date, buy_or_sell, stock_symbol, stock_name, stock_price, number_of_shares, total_price FROM users JOIN buys ON users.username = buys.username WHERE id = ?", session["user_id"])
    sell_transactions = db.execute(
        "SELECT date, buy_or_sell, stock_symbol, stock_name, stock_price, number_of_shares, total_price FROM users JOIN sells ON users.username = sells.username WHERE id = ?", session["user_id"])
    transactions = buy_transactions + sell_transactions
    transactions = sorted(buy_transactions + sell_transactions, reverse=True, key=lambda i: i["date"])

    # Change prices to have only 2 decimals and add usd sign
    for transaction in transactions:
        transaction["stock_price"] = usd(transaction["stock_price"])
        transaction["total_price"] = usd(transaction["total_price"])

    # Redirect to history page
    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""
    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        stock_info = lookup(request.form.get("symbol"))

        # Check if stock symbol is real
        if not stock_info:
            return apology("stock do not exists")

        # Redirect to to quoted (website w/ infos about stock)
        return render_template("quoted.html", name=stock_info["name"], price=usd(stock_info["price"]), symbol=stock_info["symbol"])

    # GET via clicking links
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        password_confirmation = request.form.get("confirmation")
        registered_usernames = db.execute("SELECT username FROM users")

        # Check for errors (username taken)
        for registered_username in registered_usernames:
            if re.sub(' +', ' ', username).lower().strip() == re.sub(' +', ' ', registered_username["username"]).lower().strip():
                return apology("username is already taken")

        # Check for errors (empty username)
        if not username:
            return apology("username required")

        # Check for errors (empty password)
        if not password or not password_confirmation:
            return apology("password required")

        # Check for errors (passwords not matching)
        if password != password_confirmation:
            return apology("password do not match")

        # Check for errors (password have at least 1 capital letter, small letter, number and symbol and 6 characters long)
        password_validity = {}
        for character in password:
            if character in ascii_lowercase:
                password_validity["lowercase"] = True
            elif character in ascii_uppercase:
                password_validity["uppercase"] = True
            elif character in punctuation:
                password_validity["symbol"] = True
            elif character in digits:
                password_validity["number"] = True
            password_validity["length"] = password_validity.get("length", 0) + 1

        if len(password_validity) != 5:
            return apology("password must at least have a lowercase letter, uppercase letter, number, and symbol")
        if password_validity["length"] < 6:
            return apology("password must be at least 6 characters long")

        # Add the new user to into registered users
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, generate_password_hash(password))

        # Log the user in
        row = db.execute("SELECT * FROM users WHERE username = ?", username)
        session["user_id"] = row[0]["id"]

        # Redirect to homepage
        return redirect("/")

    # GET via clicking links
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        # Get the owned stocks
        stock_info = lookup(request.form.get("symbol"))
        no_shares = request.form.get("shares")
        user_id = session["user_id"]
        username = db.execute("SELECT username FROM users WHERE id = ?", user_id)[0]["username"]

        # Check for errors (empty stocks or it do not exists)
        if not stock_info or stock_info is None:
            return apology("stock do not exists")

        # Get all the stocks bought
        bought_stocks = db.execute(
            "SELECT users.username, stock_name, stock_symbol, SUM(number_of_shares) AS number_of_shares, cash FROM users JOIN buys ON users.username = buys.username WHERE id = ? AND stock_symbol = ? GROUP BY stock_symbol", session["user_id"], stock_info["symbol"])

        # Check for errors (stock entered is not owned)
        if not bought_stocks:
            return apology("stocks not owned")

        # Check for errors (no_shares is non positive integers)
        if not no_shares.isnumeric():
            return apology("number of shares must be positive")
        no_shares = int(no_shares)
        if no_shares <= 0:
            return apology("number of shares must be positive")

        # Update bought_stocks to correctly owned stocks
        sold_stock = db.execute(
            "SELECT username, stock_name, stock_symbol, SUM(number_of_shares) AS number_of_shares FROM sells WHERE username = ? AND stock_symbol = ?", username, stock_info["symbol"])
        if sold_stock[0]["username"] is not None:
            if len(sold_stock) > 0:
                bought_stocks[0]["number_of_shares"] -= sold_stock[0]["number_of_shares"]

        # Check for errors (not enough shares is owned)
        if bought_stocks[0]["number_of_shares"] < no_shares:
            return apology("not enough shares to sell")

        # Update database
        user_balance = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]
        total_price = stock_info["price"] * no_shares
        db.execute("INSERT INTO sells VALUES (?, ?, ?, ?, ?, ?, ?, 'Sell')", datetime.now(), username,
                   stock_info["name"], stock_info["symbol"], stock_info["price"], no_shares, total_price)
        db.execute("UPDATE users SET cash = ? WHERE id = ?", user_balance + total_price, user_id)

        # Redirect to receipt
        return render_template("sold.html", symbol=stock_info["symbol"], no_shares=no_shares, price=usd(stock_info["price"]), total=usd(total_price), balance=usd(user_balance + total_price))

    # GET via clicking links
    else:
        # Get owned stocks
        owned_stocks = db.execute(
            "SELECT users.username, stock_name, stock_symbol, SUM(number_of_shares) AS number_of_shares, cash FROM users JOIN buys ON users.username = buys.username WHERE id = ? GROUP BY stock_symbol", session["user_id"])

        # Loop through owned stocks and change the number of shares to be correct
        for stock in owned_stocks:
            sold_stock = db.execute(
                "SELECT username, stock_name, stock_symbol, SUM(number_of_shares) AS number_of_shares FROM sells WHERE username = ? AND stock_symbol = ?", stock["username"], stock["stock_symbol"])

            # Subtract the sells to buys to reflect the correct number of shares owned
            if sold_stock[0]["username"] is not None:
                if len(sold_stock) > 0:
                    stock["number_of_shares"] -= sold_stock[0]["number_of_shares"]

        # Remove stocks that is no longer owned
        owned_stocks = list(filter(is_Owned, owned_stocks))

        # Redirect to sell page
        return render_template("sell.html", owned_stocks=owned_stocks)


# Added by student
@app.route("/account")
@login_required
def account():
    """Manage account details"""
    user_info = db.execute("SELECT username, cash FROM users WHERE id = ?", session["user_id"])
    return render_template("account.html", username=user_info[0]["username"], cash=usd(user_info[0]["cash"]))


# Added by student
@app.route("/password", methods=["GET", "POST"])
@login_required
def password():
    """Change user's password"""
    if request.method == "POST":
        user_info = db.execute("SELECT username, hash FROM users WHERE id = ?", session["user_id"])
        username = request.form.get("username")
        current_password = request.form.get("current_password")
        new_password = request.form.get("new_password")

        # Check for errors (empty username)
        if not username:
            return apology("must provide username")

        # Check for errors (empty password)
        if not current_password or not new_password:
            return apology("must provide password")

        # Check for errors (incorrect username)
        if username != user_info[0]["username"]:
            return apology("incorrect username")

        # Check for errors (incorrect password)
        if not check_password_hash(user_info[0]["hash"], current_password):
            return apology("incorrect password")

        # Check for errors (same password)
        if current_password == new_password:
            return apology("must use different password")

        # Check for errors (password have at least 1 capital letter, small letter, number and symbol and 6 characters long)
        password_validity = {}
        for character in new_password:
            if character in ascii_lowercase:
                password_validity["lowercase"] = True
            elif character in ascii_uppercase:
                password_validity["uppercase"] = True
            elif character in punctuation:
                password_validity["symbol"] = True
            elif character in digits:
                password_validity["number"] = True
            password_validity["length"] = password_validity.get("length", 0) + 1

        if len(password_validity) != 5:
            return apology("password must at least have a lowercase letter, uppercase letter, number, and symbol")
        if password_validity["length"] < 6:
            return apology("password must be at least 6 characters long")

        # Update the database
        db.execute("UPDATE users SET hash = ? WHERE id = ?", generate_password_hash(new_password), session["user_id"])

        # Redirect to homepage
        return redirect("/")

    # GET via links
    else:
        return render_template("password.html")


# Added by student
@app.route("/load", methods=["GET", "POST"])
@login_required
def load():
    """Load cash to balance"""
    if request.method == "POST":
        amount = int(request.form.get("amount"))
        previous_balance = db.execute("SELECT username, cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]

        # Check for errors (amoun is too little or too much)
        if amount > 100000:
            return apology("Cannot load more than $100,000")
        if amount < 1000:
            return apology("Cannot load less than $1,000")

        # Update the cash of the user in the database
        db.execute("UPDATE users SET cash = ? WHERE id = ?", previous_balance + amount, session["user_id"])

        # Redirect to receipt
        return render_template("loaded.html", previous_balance=usd(previous_balance), load=usd(amount), new_balance=usd(previous_balance + amount))

    # GET via clicking links
    else:
        balance = db.execute("SELECT username, cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
        return render_template("load.html", balance=usd(balance))
