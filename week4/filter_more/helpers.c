#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop through each pixel in the pic
    // Loop through each column
    for (int i = 0; i < height; i++)
    {
        // Loop through each row
        for (int j = 0; j < width; j++)
        {
            // Get average rgb values of the pixel
            double deci_ave = (image[i][j].rgbtRed + image[i][j].rgbtBlue + image[i][j].rgbtGreen) / 3.0;

            // Round the average
            int int_ave = round(deci_ave);

            // Change the color of the pixel
            image[i][j].rgbtRed = int_ave;
            image[i][j].rgbtBlue = int_ave;
            image[i][j].rgbtGreen = int_ave;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Temporary variables
    int max = width / 2;
    int tmpR[max];
    int tmpB[max];
    int tmpG[max];

    // Loop through half of pic
    // Loop through each column
    for (int i = 0; i < height; i++)
    {
        // Loop through half of the rows
        for (int j = 0; j < max; j++)
        {
            // Save rgb values to temporary var
            tmpR[j] = image[i][j].rgbtRed;
            tmpB[j] = image[i][j].rgbtBlue;
            tmpG[j] = image[i][j].rgbtGreen;

            // Change current rgb values to the one opposite them
            image[i][j].rgbtRed = image[i][(width - 1) - j].rgbtRed;
            image[i][j].rgbtBlue = image[i][(width - 1) - j].rgbtBlue;
            image[i][j].rgbtGreen = image[i][(width - 1) - j].rgbtGreen;

            // Change the rgb values of the opposite ones to the current
            image[i][(width - 1) - j].rgbtRed = tmpR[j];
            image[i][(width - 1) - j].rgbtBlue = tmpB[j];
            image[i][(width - 1) - j].rgbtGreen = tmpG[j];
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Variables
    double deci_ave_red, deci_ave_blue, deci_ave_green;
    int tmpR[height][width];
    int tmpB[height][width];
    int tmpG[height][width];

    // Replicate the original picture
    // Loop through each column
    for (int k = 0; k < height; k++)
    {
        // Loop through each row
        for (int l = 0; l < width; l++)
        {
            // Save to temporary array
            tmpR[k][l] = image[k][l].rgbtRed;
            tmpB[k][l] = image[k][l].rgbtBlue;
            tmpG[k][l] = image[k][l].rgbtGreen;
        }
    }

    // Loop through each pixel in the pic
    // Loop through each column
    for (int i = 0; i < height; i++)
    {
        // Loop through each row
        for (int j = 0; j < width; j++)
        {
            // If the pixel is in the corner
            if ((i == 0 && j == 0) || (i == 0 && j == (width - 1)) ||
                (i == (height - 1) && j == 0) || (i == (height - 1) && j == (width - 1)))
            {
                // If pixel is upper left corner
                if (i == 0 && j == 0)
                {
                    // Get average of each rgb values
                    deci_ave_red = (tmpR[i][j] + tmpR[i][j + 1]
                                    + tmpR[i + 1][j] + tmpR[i + 1][j + 1]) / 4.0;
                    deci_ave_blue = (tmpB[i][j] + tmpB[i][j + 1]
                                    + tmpB[i + 1][j] + tmpB[i + 1][j + 1]) / 4.0;
                    deci_ave_green = (tmpG[i][j] + tmpG[i][j + 1]
                                    + tmpG[i + 1][j] + tmpG[i + 1][j + 1]) / 4.0;
                }
                // If pixel is upper right corner
                else if (i == 0 && j == (width - 1))
                {
                    // Get average of each rgb values
                    deci_ave_red = (tmpR[i][j - 1] + tmpR[i][j]
                                    + tmpR[i + 1][j - 1] + tmpR[i + 1][j]) / 4.0;
                    deci_ave_blue = (tmpB[i][j - 1] + tmpB[i][j]
                                    + tmpB[i + 1][j - 1] + tmpB[i + 1][j]) / 4.0;
                    deci_ave_green = (tmpG[i][j - 1] + tmpG[i][j]
                                    + tmpG[i + 1][j - 1] + tmpG[i + 1][j]) / 4.0;
                }
                // If pixel is lower right corner
                else if (i == (height - 1) && j == 0)
                {
                    // Get average of each rgb values
                    deci_ave_red = (tmpR[i - 1][j] + tmpR[i - 1][j + 1]
                                    + tmpR[i][j] + tmpR[i][j + 1]) / 4.0;
                    deci_ave_blue = (tmpB[i - 1][j] + tmpB[i - 1][j + 1]
                                    + tmpB[i][j] + tmpB[i][j + 1]) / 4.0;
                    deci_ave_green = (tmpG[i - 1][j] + tmpG[i - 1][j + 1]
                                    + tmpG[i][j] + tmpG[i][j + 1]) / 4.0;
                }
                // If pixel is lower left corner
                else
                {
                    // Get average of each rgb values
                    deci_ave_red = (tmpR[i - 1][j - 1] + tmpR[i - 1][j]
                                    + tmpR[i][j - 1] + tmpR[i][j]) / 4.0;
                    deci_ave_blue = (tmpB[i - 1][j - 1] + tmpB[i - 1][j]
                                    + tmpB[i][j - 1] + tmpB[i][j]) / 4.0;
                    deci_ave_green = (tmpG[i - 1][j - 1] + tmpG[i - 1][j]
                                    + tmpG[i][j - 1] + tmpG[i][j]) / 4.0;
                }

            }


            // If pixel is at the edge but not corner
            else if (i == 0 || i == (height - 1) || j == 0 || j == (width - 1))
            {
                // If pixel is at top edge
                if (i == 0)
                {
                    // Get average of each rgb values
                    deci_ave_red = (tmpR[i][j - 1] + tmpR[i][j] + tmpR[i][j + 1]
                                    + tmpR[i + 1][j - 1] + tmpR[i + 1][j] + tmpR[i + 1][j + 1]) / 6.0;

                    deci_ave_blue = (tmpB[i][j - 1] + tmpB[i][j] + tmpB[i][j + 1]
                                    + tmpB[i + 1][j - 1] + tmpB[i + 1][j] + tmpB[i + 1][j + 1]) / 6.0;

                    deci_ave_green = (tmpG[i][j - 1] + tmpG[i][j] + tmpG[i][j + 1]
                                    + tmpG[i + 1][j - 1] + tmpG[i + 1][j] + tmpG[i + 1][j + 1]) / 6.0;
                }
                // If pixel is at bottom edge
                else if (i == (height - 1))
                {
                    // Get average of each rgb values
                    deci_ave_red = (tmpR[i - 1][j - 1] + tmpR[i - 1][j] + tmpR[i - 1][j + 1]
                                    + tmpR[i][j - 1] + tmpR[i][j] + tmpR[i][j + 1]) / 6.0;

                    deci_ave_blue = (tmpB[i - 1][j - 1] + tmpB[i - 1][j] + tmpB[i - 1][j + 1]
                                    + tmpB[i][j - 1] + tmpB[i][j] + tmpB[i][j + 1]) / 6.0;

                    deci_ave_green = (tmpG[i - 1][j - 1] + tmpG[i - 1][j] + tmpG[i - 1][j + 1]
                                    + tmpG[i][j - 1] + tmpG[i][j] + tmpG[i][j + 1]) / 6.0;
                }

                // If pixel is at right edge
                else if (j == 0)
                {
                    // Get average of each rgb values
                    deci_ave_red = (tmpR[i - 1][j] + tmpR[i - 1][j + 1]
                                    + tmpR[i][j] + tmpR[i][j + 1]
                                    + tmpR[i + 1][j] + tmpR[i + 1][j + 1]) / 6.0;

                    deci_ave_blue = (tmpB[i - 1][j] + tmpB[i - 1][j + 1]
                                    + tmpB[i][j] + tmpB[i][j + 1]
                                    + tmpB[i + 1][j] + tmpB[i + 1][j + 1]) / 6.0;

                    deci_ave_green = (tmpG[i - 1][j] + tmpG[i - 1][j + 1]
                                    + tmpG[i][j] + tmpG[i][j + 1]
                                    + tmpG[i + 1][j] + tmpG[i + 1][j + 1]) / 6.0;
                }
                // If pixel is at left edge
                else
                {
                    // Get average of each rgb values
                    deci_ave_red = (tmpR[i - 1][j - 1] + tmpR[i - 1][j]
                                    + tmpR[i][j - 1] + tmpR[i][j]
                                    + tmpR[i + 1][j - 1] + tmpR[i + 1][j]) / 6.0;

                    deci_ave_blue = (tmpB[i - 1][j - 1] + tmpB[i - 1][j]
                                    + tmpB[i][j - 1] + tmpB[i][j]
                                    + tmpB[i + 1][j - 1] + tmpB[i + 1][j]) / 6.0;

                    deci_ave_green = (tmpG[i - 1][j - 1] + tmpG[i - 1][j]
                                    + tmpG[i][j - 1] + tmpG[i][j]
                                    + tmpG[i + 1][j - 1] + tmpG[i + 1][j]) / 6.0;
                }

            }

            // If pixel is at center (not edge and corner)
            else
            {
                // Get average of each rgb values
                deci_ave_red = (tmpR[i - 1][j - 1] + tmpR[i - 1][j] + tmpR[i - 1][j + 1]
                                + tmpR[i][j - 1] + tmpR[i][j] + tmpR[i][j + 1]
                                + tmpR[i + 1][j - 1] + tmpR[i + 1][j] + tmpR[i + 1][j + 1]) / 9.0;

                deci_ave_blue = (tmpB[i - 1][j - 1] + tmpB[i - 1][j] + tmpB[i - 1][j + 1]
                                + tmpB[i][j - 1] + tmpB[i][j] + tmpB[i][j + 1]
                                + tmpB[i + 1][j - 1] + tmpB[i + 1][j] + tmpB[i + 1][j + 1]) / 9.0;

                deci_ave_green = (tmpG[i - 1][j - 1] + tmpG[i - 1][j] + tmpG[i - 1][j + 1]
                                + tmpG[i][j - 1] + tmpG[i][j] + tmpG[i][j + 1]
                                + tmpG[i + 1][j - 1] + tmpG[i + 1][j] + tmpG[i + 1][j + 1]) / 9.0;
            }

            // Round the average rgb values to nearest int then change the color of pixel
            image[i][j].rgbtRed = round(deci_ave_red);
            image[i][j].rgbtBlue = round(deci_ave_blue);
            image[i][j].rgbtGreen = round(deci_ave_green);
        }


    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Save Gx and Gy matrix to array for easier use
    const int MAX = 3;
    int Gx[MAX][MAX] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[MAX][MAX] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // Variables
    double sumR_Gx, sumB_Gx, sumG_Gx, sumR_Gy, sumB_Gy, sumG_Gy;
    double tmpR_int, tmpB_int, tmpG_int;
    int tmpR[height][width];
    int tmpB[height][width];
    int tmpG[height][width];

    // Replicate the original pic
    // Loop through each column
    for (int k = 0; k < height; k++)
    {
        // Loop through each row
        for (int l = 0; l < width; l++)
        {
            // Save rgb values to temporary var
            tmpR[k][l] = image[k][l].rgbtRed;
            tmpB[k][l] = image[k][l].rgbtBlue;
            tmpG[k][l] = image[k][l].rgbtGreen;
        }
    }

    // Loop through each pixel
    // Loop through each column
    for (int i = 0; i < height; i++)
    {
        // Loop through each row
        for (int j = 0; j < width; j++)
        {
            // If pixel is in the corner
            if ((i == 0 && j == 0) || (i == 0 && j == (width - 1)) ||
                (i == (height - 1) && j == 0) || (i == (height - 1) && j == (width - 1)))
            {
                // If pixel is in the upper left corner
                if (i == 0 && j == 0)
                {
                    // Get rgb values using Gx matrix
                    sumR_Gx = ((tmpR[i][j] * Gx[1][1]) + (tmpR[i][j + 1] * Gx[1][2])
                               + (tmpR[i + 1][j] * Gx[2][1]) + (tmpR[i + 1][j + 1] * Gx[2][2]));

                    sumB_Gx = ((tmpB[i][j] * Gx[1][1]) + (tmpB[i][j + 1] * Gx[1][2])
                               + (tmpB[i + 1][j] * Gx[2][1]) + (tmpB[i + 1][j + 1] * Gx[2][2]));

                    sumG_Gx = ((tmpG[i][j] * Gx[1][1]) + (tmpG[i][j + 1] * Gx[1][2])
                               + (tmpG[i + 1][j] * Gx[2][1]) + (tmpG[i + 1][j + 1] * Gx[2][2]));

                    // Get rgb values using Gy matrix
                    sumR_Gy = ((tmpR[i][j] * Gy[1][1]) + (tmpR[i][j + 1] * Gy[1][2])
                               + (tmpR[i + 1][j] * Gy[2][1]) + (tmpR[i + 1][j + 1] * Gy[2][2]));

                    sumB_Gy = ((tmpB[i][j] * Gy[1][1]) + (tmpB[i][j + 1] * Gy[1][2])
                               + (tmpB[i + 1][j] * Gy[2][1]) + (tmpB[i + 1][j + 1] * Gy[2][2]));

                    sumG_Gy = ((tmpG[i][j] * Gy[1][1]) + (tmpG[i][j + 1] * Gy[1][2])
                               + (tmpG[i + 1][j] * Gy[2][1]) + (tmpG[i + 1][j + 1] * Gy[2][2]));

                }
                // If pixel is in the upper right corner
                else if (i == 0 && j == (width - 1))
                {
                    // Get rgb values using Gx matrix
                    sumR_Gx = ((tmpR[i][j - 1] * Gx[1][0]) + (tmpR[i][j] * Gx[1][1])
                               + (tmpR[i + 1][j - 1] * Gx[2][0]) + (tmpR[i + 1][j] * Gx[2][1]));

                    sumB_Gx = ((tmpB[i][j - 1] * Gx[1][0]) + (tmpB[i][j] * Gx[1][1])
                               + (tmpB[i + 1][j - 1] * Gx[2][0]) + (tmpB[i + 1][j] * Gx[2][1]));

                    sumG_Gx = ((tmpG[i][j - 1] * Gx[1][0]) + (tmpG[i][j] * Gx[1][1])
                               + (tmpG[i + 1][j - 1] * Gx[2][0]) + (tmpG[i + 1][j] * Gx[2][1]));

                    // Get rgb values using Gy matrix
                    sumR_Gy = ((tmpR[i][j - 1] * Gy[1][0]) + (tmpR[i][j] * Gy[1][1])
                               + (tmpR[i + 1][j - 1] * Gy[2][0]) + (tmpR[i + 1][j] * Gy[2][1]));

                    sumB_Gy = ((tmpB[i][j - 1] * Gy[1][0]) + (tmpB[i][j] * Gy[1][1])
                               + (tmpB[i + 1][j - 1] * Gy[2][0]) + (tmpB[i + 1][j] * Gy[2][1]));

                    sumG_Gy = ((tmpG[i][j - 1] * Gy[1][0]) + (tmpG[i][j] * Gy[1][1])
                               + (tmpG[i + 1][j - 1] * Gy[2][0]) + (tmpG[i + 1][j] * Gy[2][1]));

                }
                // If pixel is in the lower left corner
                else if (i == (height - 1) && j == 0)
                {
                    // Get rgb values using Gx matrix
                    sumR_Gx = ((tmpR[i - 1][j] * Gx[0][1]) + (tmpR[i - 1][j + 1] * Gx[0][2])
                               + (tmpR[i][j] * Gx[1][1]) + (tmpR[i][j + 1] * Gx[1][2]));

                    sumB_Gx = ((tmpB[i - 1][j] * Gx[0][1]) + (tmpB[i - 1][j + 1] * Gx[0][2])
                               + (tmpB[i][j] * Gx[1][1]) + (tmpB[i][j + 1] * Gx[1][2]));

                    sumG_Gx = ((tmpG[i - 1][j] * Gx[0][1]) + (tmpG[i - 1][j + 1] * Gx[0][2])
                               + (tmpG[i][j] * Gx[1][1]) + (tmpG[i][j + 1] * Gx[1][2]));

                    // Get rgb values using Gy matrix
                    sumR_Gy = ((tmpR[i - 1][j] * Gy[0][1]) + (tmpR[i - 1][j + 1] * Gy[0][2])
                               + (tmpR[i][j] * Gy[1][1]) + (tmpR[i][j + 1] * Gy[1][2]));

                    sumB_Gy = ((tmpB[i - 1][j] * Gy[0][1]) + (tmpB[i - 1][j + 1] * Gy[0][2])
                               + (tmpB[i][j] * Gy[1][1]) + (tmpB[i][j + 1] * Gy[1][2]));

                    sumG_Gy = ((tmpG[i - 1][j] * Gy[0][1]) + (tmpG[i - 1][j + 1] * Gy[0][2])
                               + (tmpG[i][j] * Gy[1][1]) + (tmpG[i][j + 1] * Gy[1][2]));
                }

                // If pixel is in the lower right corner
                else
                {
                    // Get rgb values using Gx matrix
                    sumR_Gx = ((tmpR[i - 1][j - 1] * Gx[0][0]) + (tmpR[i - 1][j] * Gx[0][1])
                               + (tmpR[i][j - 1] * Gx[1][0]) + (tmpR[i][j] * Gx[1][1]));

                    sumB_Gx = ((tmpB[i - 1][j - 1] * Gx[0][0]) + (tmpB[i - 1][j] * Gx[0][1])
                               + (tmpB[i][j - 1] * Gx[1][0]) + (tmpB[i][j] * Gx[1][1]));

                    sumG_Gx = ((tmpG[i - 1][j - 1] * Gx[0][0]) + (tmpG[i - 1][j] * Gx[0][1])
                               + (tmpG[i][j - 1] * Gx[1][0]) + (tmpG[i][j] * Gx[1][1]));

                    // Get rgb values using Gy matrix
                    sumR_Gy = ((tmpR[i - 1][j - 1] * Gy[0][0]) + (tmpR[i - 1][j] * Gy[0][1])
                               + (tmpR[i][j - 1] * Gy[1][0]) + (tmpR[i][j] * Gy[1][1]));

                    sumB_Gy = ((tmpB[i - 1][j - 1] * Gy[0][0]) + (tmpB[i - 1][j] * Gy[0][1])
                               + (tmpB[i][j - 1] * Gy[1][0]) + (tmpB[i][j] * Gy[1][1]));

                    sumG_Gy = ((tmpG[i - 1][j - 1] * Gy[0][0]) + (tmpG[i - 1][j] * Gy[0][1])
                               + (tmpG[i][j - 1] * Gy[1][0]) + (tmpG[i][j] * Gy[1][1]));
                }
            }

            // If pixel is at the edge but not corner
            else if (i == 0 || i == (height - 1) || j == 0 || j == (width - 1))
            {
                // If pixel is at top edge
                if (i == 0)
                {
                    // Get rgb values using Gx matrix
                    sumR_Gx = ((tmpR[i][j - 1] * Gx[1][0]) + (tmpR[i][j] * Gx[1][1]) + (tmpR[i][j + 1] * Gx[1][2])
                               + (tmpR[i + 1][j - 1] * Gx[2][0]) + (tmpR[i + 1][j] * Gx[2][1]) + (tmpR[i + 1][j + 1] * Gx[2][2]));

                    sumB_Gx = ((tmpB[i][j - 1] * Gx[1][0]) + (tmpB[i][j] * Gx[1][1]) + (tmpB[i][j + 1] * Gx[1][2])
                               + (tmpB[i + 1][j - 1] * Gx[2][0]) + (tmpB[i + 1][j] * Gx[2][1]) + (tmpB[i + 1][j + 1] * Gx[2][2]));

                    sumG_Gx = ((tmpG[i][j - 1] * Gx[1][0]) + (tmpG[i][j] * Gx[1][1]) + (tmpG[i][j + 1] * Gx[1][2])
                               + (tmpG[i + 1][j - 1] * Gx[2][0]) + (tmpG[i + 1][j] * Gx[2][1]) + (tmpG[i + 1][j + 1] * Gx[2][2]));

                    // Get rgb values using Gy matrix
                    sumR_Gy = ((tmpR[i][j - 1] * Gy[1][0]) + (tmpR[i][j] * Gy[1][1]) + (tmpR[i][j + 1] * Gy[1][2])
                               + (tmpR[i + 1][j - 1] * Gy[2][0]) + (tmpR[i + 1][j] * Gy[2][1]) + (tmpR[i + 1][j + 1] * Gy[2][2]));

                    sumB_Gy = ((tmpB[i][j - 1] * Gy[1][0]) + (tmpB[i][j] * Gy[1][1]) + (tmpB[i][j + 1] * Gy[1][2])
                               + (tmpB[i + 1][j - 1] * Gy[2][0]) + (tmpB[i + 1][j] * Gy[2][1]) + (tmpB[i + 1][j + 1] * Gy[2][2]));

                    sumG_Gy = ((tmpG[i][j - 1] * Gy[1][0]) + (tmpG[i][j] * Gy[1][1]) + (tmpG[i][j + 1] * Gy[1][2])
                               + (tmpG[i + 1][j - 1] * Gy[2][0]) + (tmpG[i + 1][j] * Gy[2][1]) + (tmpG[i + 1][j + 1] * Gy[2][2]));

                }
                // If pixel is at bottom edge
                else if (i == (height - 1))
                {
                    // Get rgb values using Gx matrix
                    sumR_Gx = ((tmpR[i - 1][j - 1] * Gx[0][0]) + (tmpR[i - 1][j] * Gx[0][1]) + (tmpR[i - 1][j + 1] * Gx[0][2])
                               + (tmpR[i][j - 1] * Gx[1][0]) + (tmpR[i][j] * Gx[1][1]) + (tmpR[i][j + 1] * Gx[1][2]));

                    sumB_Gx = ((tmpB[i - 1][j - 1] * Gx[0][0]) + (tmpB[i - 1][j] * Gx[0][1]) + (tmpB[i - 1][j + 1] * Gx[0][2])
                               + (tmpB[i][j - 1] * Gx[1][0]) + (tmpB[i][j] * Gx[1][1]) + (tmpB[i][j + 1] * Gx[1][2]));

                    sumG_Gx = ((tmpG[i - 1][j - 1] * Gx[0][0]) + (tmpG[i - 1][j] * Gx[0][1]) + (tmpG[i - 1][j + 1] * Gx[0][2])
                               + (tmpG[i][j - 1] * Gx[1][0]) + (tmpG[i][j] * Gx[1][1]) + (tmpG[i][j + 1] * Gx[1][2]));

                    // Get rgb values using Gy matrix
                    sumR_Gy = ((tmpR[i - 1][j - 1] * Gy[0][0]) + (tmpR[i - 1][j] * Gy[0][1]) + (tmpR[i - 1][j + 1] * Gy[0][2])
                               + (tmpR[i][j - 1] * Gy[1][0]) + (tmpR[i][j] * Gy[1][1]) + (tmpR[i][j + 1] * Gy[1][2]));

                    sumB_Gy = ((tmpB[i - 1][j - 1] * Gy[0][0]) + (tmpB[i - 1][j] * Gy[0][1]) + (tmpB[i - 1][j + 1] * Gy[0][2])
                               + (tmpB[i][j - 1] * Gy[1][0]) + (tmpB[i][j] * Gy[1][1]) + (tmpB[i][j + 1] * Gy[1][2]));

                    sumG_Gy = ((tmpG[i - 1][j - 1] * Gy[0][0]) + (tmpG[i - 1][j] * Gy[0][1]) + (tmpG[i - 1][j + 1] * Gy[0][2])
                               + (tmpG[i][j - 1] * Gy[1][0]) + (tmpG[i][j] * Gy[1][1]) + (tmpG[i][j + 1] * Gy[1][2]));
                }

                // If pixel is at left edge
                else if (j == 0)
                {
                    // Get rgb values using Gx matrix
                    sumR_Gx = ((tmpR[i - 1][j] * Gx[0][1]) + (tmpR[i - 1][j + 1] * Gx[0][2])
                               + (tmpR[i][j] * Gx[1][1]) + (tmpR[i][j + 1] * Gx[1][2])
                               + (tmpR[i + 1][j] * Gx[2][1]) + (tmpR[i + 1][j + 1] * Gx[2][2]));

                    sumB_Gx = ((tmpB[i - 1][j] * Gx[0][1]) + (tmpB[i - 1][j + 1] * Gx[0][2])
                               + (tmpB[i][j] * Gx[1][1]) + (tmpB[i][j + 1] * Gx[1][2])
                               + (tmpB[i + 1][j] * Gx[2][1]) + (tmpB[i + 1][j + 1] * Gx[2][2]));

                    sumG_Gx = ((tmpG[i - 1][j] * Gx[0][1]) + (tmpG[i - 1][j + 1] * Gx[0][2])
                               + (tmpG[i][j] * Gx[1][1]) + (tmpG[i][j + 1] * Gx[1][2])
                               + (tmpG[i + 1][j] * Gx[2][1]) + (tmpG[i + 1][j + 1] * Gx[2][2]));

                    // Get rgb values using Gy matrix
                    sumR_Gy = ((tmpR[i - 1][j] * Gy[0][1]) + (tmpR[i - 1][j + 1] * Gy[0][2])
                               + (tmpR[i][j] * Gy[1][1]) + (tmpR[i][j + 1] * Gy[1][2])
                               + (tmpR[i + 1][j] * Gy[2][1]) + (tmpR[i + 1][j + 1] * Gy[2][2]));

                    sumB_Gy = ((tmpB[i - 1][j] * Gy[0][1]) + (tmpB[i - 1][j + 1] * Gy[0][2])
                               + (tmpB[i][j] * Gy[1][1]) + (tmpB[i][j + 1] * Gy[1][2])
                               + (tmpB[i + 1][j] * Gy[2][1]) + (tmpB[i + 1][j + 1] * Gy[2][2]));

                    sumG_Gy = ((tmpG[i - 1][j] * Gy[0][1]) + (tmpG[i - 1][j + 1] * Gy[0][2])
                               + (tmpG[i][j] * Gy[1][1]) + (tmpG[i][j + 1] * Gy[1][2])
                               + (tmpG[i + 1][j] * Gy[2][1]) + (tmpG[i + 1][j + 1] * Gy[2][2]));
                }

                // If pixel is at right edge
                else
                {
                    // Get rgb values using Gx matrix
                    sumR_Gx = ((tmpR[i - 1][j - 1] * Gx[0][0]) + (tmpR[i - 1][j] * Gx[0][1])
                               + (tmpR[i][j - 1] * Gx[1][0]) + (tmpR[i][j] * Gx[1][1])
                               + (tmpR[i + 1][j - 1] * Gx[2][0]) + (tmpR[i + 1][j] * Gx[2][1]));

                    sumB_Gx = ((tmpB[i - 1][j - 1] * Gx[0][0]) + (tmpB[i - 1][j] * Gx[0][1])
                               + (tmpB[i][j - 1] * Gx[1][0]) + (tmpB[i][j] * Gx[1][1])
                               + (tmpB[i + 1][j - 1] * Gx[2][0]) + (tmpB[i + 1][j] * Gx[2][1]));

                    sumG_Gx = ((tmpG[i - 1][j - 1] * Gx[0][0]) + (tmpG[i - 1][j] * Gx[0][1])
                               + (tmpG[i][j - 1] * Gx[1][0]) + (tmpG[i][j] * Gx[1][1])
                               + (tmpG[i + 1][j - 1] * Gx[2][0]) + (tmpG[i + 1][j] * Gx[2][1]));

                    // Get rgb values using Gy matrix
                    sumR_Gy = ((tmpR[i - 1][j - 1] * Gy[0][0]) + (tmpR[i - 1][j] * Gy[0][1])
                               + (tmpR[i][j - 1] * Gy[1][0]) + (tmpR[i][j] * Gy[1][1])
                               + (tmpR[i + 1][j - 1] * Gy[2][0]) + (tmpR[i + 1][j] * Gy[2][1]));

                    sumB_Gy = ((tmpB[i - 1][j - 1] * Gy[0][0]) + (tmpB[i - 1][j] * Gy[0][1])
                               + (tmpB[i][j - 1] * Gy[1][0]) + (tmpB[i][j] * Gy[1][1])
                               + (tmpB[i + 1][j - 1] * Gy[2][0]) + (tmpB[i + 1][j] * Gy[2][1]));

                    sumG_Gy = ((tmpG[i - 1][j - 1] * Gy[0][0]) + (tmpG[i - 1][j] * Gy[0][1])
                               + (tmpG[i][j - 1] * Gy[1][0]) + (tmpG[i][j] * Gy[1][1])
                               + (tmpG[i + 1][j - 1] * Gy[2][0]) + (tmpG[i + 1][j] * Gy[2][1]));
                }

            }

            // If pixel is at the center (not corner or edge)
            else
            {
                // Get rgb values using Gx matrix
                sumR_Gx = ((tmpR[i - 1][j - 1] * Gx[0][0]) + (tmpR[i - 1][j] * Gx[0][1]) + (tmpR[i - 1][j + 1] * Gx[0][2])
                           + (tmpR[i][j - 1] * Gx[1][0]) + (tmpR[i][j] * Gx[1][1]) + (tmpR[i][j + 1] * Gx[1][2])
                           + (tmpR[i + 1][j - 1] * Gx[2][0]) + (tmpR[i + 1][j] * Gx[2][1]) + (tmpR[i + 1][j + 1] * Gx[2][2]));

                sumB_Gx = ((tmpB[i - 1][j - 1] * Gx[0][0]) + (tmpB[i - 1][j] * Gx[0][1]) + (tmpB[i - 1][j + 1] * Gx[0][2])
                           + (tmpB[i][j - 1] * Gx[1][0]) + (tmpB[i][j] * Gx[1][1]) + (tmpB[i][j + 1] * Gx[1][2])
                           + (tmpB[i + 1][j - 1] * Gx[2][0]) + (tmpB[i + 1][j] * Gx[2][1]) + (tmpB[i + 1][j + 1] * Gx[2][2]));

                sumG_Gx = ((tmpG[i - 1][j - 1] * Gx[0][0]) + (tmpG[i - 1][j] * Gx[0][1]) + (tmpG[i - 1][j + 1] * Gx[0][2])
                           + (tmpG[i][j - 1] * Gx[1][0]) + (tmpG[i][j] * Gx[1][1]) + (tmpG[i][j + 1] * Gx[1][2])
                           + (tmpG[i + 1][j - 1] * Gx[2][0]) + (tmpG[i + 1][j] * Gx[2][1]) + (tmpG[i + 1][j + 1] * Gx[2][2]));

                // Get rgb values using Gy matrix
                sumR_Gy = ((tmpR[i - 1][j - 1] * Gy[0][0]) + (tmpR[i - 1][j] * Gy[0][1]) + (tmpR[i - 1][j + 1] * Gy[0][2])
                           + (tmpR[i][j - 1] * Gy[1][0]) + (tmpR[i][j] * Gy[1][1]) + (tmpR[i][j + 1] * Gy[1][2])
                           + (tmpR[i + 1][j - 1] * Gy[2][0]) + (tmpR[i + 1][j] * Gy[2][1]) + (tmpR[i + 1][j + 1] * Gy[2][2]));

                sumB_Gy = ((tmpB[i - 1][j - 1] * Gy[0][0]) + (tmpB[i - 1][j] * Gy[0][1]) + (tmpB[i - 1][j + 1] * Gy[0][2])
                           + (tmpB[i][j - 1] * Gy[1][0]) + (tmpB[i][j] * Gy[1][1]) + (tmpB[i][j + 1] * Gy[1][2])
                           + (tmpB[i + 1][j - 1] * Gy[2][0]) + (tmpB[i + 1][j] * Gy[2][1]) + (tmpB[i + 1][j + 1] * Gy[2][2]));

                sumG_Gy = ((tmpG[i - 1][j - 1] * Gy[0][0]) + (tmpG[i - 1][j] * Gy[0][1]) + (tmpG[i - 1][j + 1] * Gy[0][2])
                           + (tmpG[i][j - 1] * Gy[1][0]) + (tmpG[i][j] * Gy[1][1]) + (tmpG[i][j + 1] * Gy[1][2])
                           + (tmpG[i + 1][j - 1] * Gy[2][0]) + (tmpG[i + 1][j] * Gy[2][1]) + (tmpG[i + 1][j + 1] * Gy[2][2]));

            }

            // Use pythagorean theorem to get rgb value
            tmpR_int = sqrt(pow(sumR_Gx, 2.0) + pow(sumR_Gy, 2.0));
            tmpB_int = sqrt(pow(sumB_Gx, 2.0) + pow(sumB_Gy, 2.0));
            tmpG_int = sqrt(pow(sumG_Gx, 2.0) + pow(sumG_Gy, 2.0));

            // Cap all values to 255 and change the color
            // Red
            if (round(tmpR_int) > 255)
            {
                image[i][j].rgbtRed = 255;
            }
            else
            {
                image[i][j].rgbtRed = round(tmpR_int);
            }
            // Blue
            if (round(tmpB_int) > 255)
            {
                image[i][j].rgbtBlue = 255;
            }
            else
            {
                image[i][j].rgbtBlue = round(tmpB_int);
            }
            // Green
            if (round(tmpG_int) > 255)
            {
                image[i][j].rgbtGreen = 255;
            }
            else
            {
                image[i][j].rgbtGreen = round(tmpG_int);
            }
        }
    }
    return;
}
