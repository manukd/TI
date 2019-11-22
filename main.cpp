#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

#define N 3

cv::Mat seuillage(cv::Mat o);
cv::Mat addition(cv::Mat o1, cv::Mat o2);
cv::Mat additionBinaire(cv::Mat o1, cv::Mat o2, bool seuil);
cv::Mat soustraction(cv::Mat o1, cv::Mat o2, bool seuil);
cv::Mat soustractionBinaire(cv::Mat o1, cv::Mat o2, bool seuil);
cv::Mat erosion(cv::Mat o1, bool seuil);
cv::Mat erosion(cv::Mat o1, bool seuil, int taille);
cv::Mat dilation(cv::Mat o1, bool seuil);
cv::Mat ouverture(cv::Mat o1, bool seuil);
cv::Mat fermeture(cv::Mat o1);
cv::Mat amincissement(cv::Mat o1, bool seuil);
cv::Mat amincissementtest(cv::Mat o1, bool seuil);
cv::Mat epaississement(cv::Mat o1, bool seuil);
void rotateMatrix(uchar mat[][N]);
int countBool(bool tab[8]);
int calculSeuil(cv::Mat o1);
bool empty(cv::Mat o1);
cv::Mat lantuejoul(cv::Mat o1, bool seuil);

int main(int argc, char** argv)
{

    cv::Mat frame, frame1, grayFrame;
    frame = cv::imread("Capsule.bmp", cv::IMREAD_COLOR); // Read the file
    frame1 = cv::imread("trou-noir-espace-univers-astronomie.jpg", cv::IMREAD_COLOR); // Read the file

    if(!frame.data) {
        std::cout << "Could not open or find the frame" << std::endl;
        return -1;
    }

    cv::namedWindow("Window", cv::WINDOW_AUTOSIZE); // Create a window for display.
    /*frame = amincissement(frame, true);
    for (int i = 0; i < 100; ++i) {
        frame = amincissement(frame, false);
    }*/
    cv::imshow("Window", seuillage(frame)); // Show our image inside it.



    }
    cv::waitKey(0); // Wait for a keystroke in the window
    return 0;
}

cv::Mat seuillage(cv::Mat o) {
    cv::Mat grayFrame;
    cv::cvtColor(o, grayFrame, cv::COLOR_BGR2GRAY);
    int seuil = calculSeuil(grayFrame);
    for (int i = 0; i < grayFrame.rows; i++) {
        for (int j = 0; j < grayFrame.cols; j++) {
            if(grayFrame.at<uchar>(i,j) <= seuil) {
                grayFrame.at<uchar>(i,j) = 255;
            } else {
                grayFrame.at<uchar>(i,j) = 0;
            }
        }
    }
    return grayFrame;
}

cv::Mat addition(cv::Mat o1, cv::Mat o2) {
    cv::Mat grayFrame1, grayFrame2;
    cv::cvtColor(o1, grayFrame1, cv::COLOR_BGR2GRAY);
    cv::cvtColor(o2, grayFrame2, cv::COLOR_BGR2GRAY);
    for (int i = 0; i < grayFrame1.rows; i++) {
        for (int j = 0; j < grayFrame1.cols; j++) {
            if(grayFrame1.at<uchar>(i,j) + grayFrame2.at<uchar>(i,j) > 255) {
                grayFrame1.at<uchar>(i,j) = 255;
            } else {
                grayFrame1.at<uchar>(i,j) += grayFrame2.at<uchar>(i,j);
            }
        }
    }
    return grayFrame1;
}

cv::Mat additionBinaire(cv::Mat o1, cv::Mat o2, bool seuil) {
    cv::Mat temp1, temp2;
    if(seuil) {
        temp1 = seuillage(o1);
        temp2 = seuillage(o2);
    } else {
        temp1 = o1.clone();
        temp2 = o2.clone();
    }
    for (int i = 0; i < temp1.rows; i++) {
        for (int j = 0; j < temp1.cols; j++) {
            temp1.at<uchar>(i,j) += temp2.at<uchar>(i,j);
        }
    }
    return temp1;
}

cv::Mat soustraction(cv::Mat o1, cv::Mat o2, bool seuil) {
    cv::Mat grayFrame1, grayFrame2;
    if(seuil) {
        cv::cvtColor(o1, grayFrame1, cv::COLOR_BGR2GRAY);
        cv::cvtColor(o2, grayFrame2, cv::COLOR_BGR2GRAY);
    } else {
        grayFrame1 = o1.clone();
        grayFrame2 = o2.clone();
    }
    for (int i = 0; i < grayFrame1.rows; i++) {
        for (int j = 0; j < grayFrame1.cols; j++) {
            if(grayFrame1.at<uchar>(i,j) - grayFrame2.at<uchar>(i,j) < 0) {
                grayFrame1.at<uchar>(i,j) = 0;
            } else {
                grayFrame1.at<uchar>(i,j) -= grayFrame2.at<uchar>(i,j);
            }
        }
    }
    return grayFrame1;
}

cv::Mat soustractionBinaire(cv::Mat o1, cv::Mat o2, bool seuil) {
    cv::Mat temp1;
    cv::Mat temp2;
    if(seuil) {
        temp1 = seuillage(o1.clone());
        temp2 = seuillage(o2.clone());
    } else {
        temp1 = o1.clone();
        temp2 = o2.clone();
    }
    for (int i = 0; i < temp1.rows; i++) {
        for (int j = 0; j < temp1.cols; j++) {
            temp1.at<uchar>(i,j) -= temp2.at<uchar>(i,j);
        }
    }
    return temp1;
}

cv::Mat erosion(cv::Mat o1, bool seuil) {
    cv::Mat grayFrame1 = o1.clone();
    cv::Mat imageResult = o1.clone();
    if(seuil) {
        grayFrame1 = seuillage(o1.clone());
        imageResult = seuillage(o1.clone());
    }
    uchar filter[3][3] = {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}};
    uchar temp[3][3] = {};
    for (int i = 1; i < grayFrame1.rows - 2; ++i) {
        for (int j = 1; j < grayFrame1.cols - 2; ++j) {
            if(grayFrame1.at<uchar>(i,j) == 255) {
                temp[0][0] = grayFrame1.at<uchar>(i-1,j-1);
                temp[0][1] = grayFrame1.at<uchar>(i-1,j);
                temp[0][2] = grayFrame1.at<uchar>(i-1,j+1);

                temp[1][0] = grayFrame1.at<uchar>(i,j-1);
                temp[1][1] = grayFrame1.at<uchar>(i,j);
                temp[1][2] = grayFrame1.at<uchar>(i,j+1);

                temp[2][0] = grayFrame1.at<uchar>(i+1,j-1);
                temp[2][1] = grayFrame1.at<uchar>(i+1,j);
                temp[2][2] = grayFrame1.at<uchar>(i+1,j+1);
                bool res = true;
                for (int i = 0; i < 3; ++i) {
                    for (int j = 0; j < 3; ++j) {
                        if(temp[i][j] != filter[i][j]) {
                            res = false;
                        }
                    }
                }
                if(!res) {
                    imageResult.at<uchar>(i,j) = 0;
                }
            }
        }
    }
    return imageResult;
}

cv::Mat erosion(cv::Mat o1, bool seuil, int tailleBoule) {
    cv::Mat grayFrame1 = o1.clone();
    cv::Mat imageResult = o1.clone();
    if(seuil) {
        grayFrame1 = seuillage(o1.clone());
        imageResult = seuillage(o1.clone());
    }
    int taille = (3 + 2 * (tailleBoule - 1)) ;
    uchar filter[taille][taille];
    for (int k = 0; k <taille ; ++k) {
        for (int i = 0; i <taille ; ++i) {
            filter[k][i] = 255;
        }
    }
    uchar temp[taille][taille];
    int demiTaille = floor(taille/2);
    for (int m = 0; m < demiTaille; ++m) {
        for (int n = 0; n < o1.cols; ++n) {
            imageResult.at<uchar>(m,n) = 0;
        }
    }
    for (int m = 0; m < o1.rows; ++m) {
        for (int n = 0; n < demiTaille; ++n) {
            imageResult.at<uchar>(m,n) = 0;
        }
    }
    for (int m = o1.rows - demiTaille; m < o1.rows; ++m) {
        for (int n = 0; n < o1.cols; ++n) {
            imageResult.at<uchar>(m,n) = 0;
        }
    }
    for (int m = 0; m < o1.rows; ++m) {
        for (int n = o1.cols - demiTaille; n < o1.cols; ++n) {
            imageResult.at<uchar>(m,n) = 0;
        }
    }
    for (int i = demiTaille; i < grayFrame1.rows - demiTaille; ++i) {
        for (int j = demiTaille; j < grayFrame1.cols - demiTaille; ++j) {
            if(grayFrame1.at<uchar>(i,j) == 255) {
                bool res = true;
                for (int k = 0; k < taille; ++k) {
                    for (int l = 0; l < taille; ++l) {
                        temp[k][l] = grayFrame1.at<uchar>(i-demiTaille+k,j-demiTaille+l);
                        if(temp[k][l] != filter[k][l]) {
                            res = false;
                        }
                    }
                }
                if(!res) {
                    imageResult.at<uchar>(i,j) = 0;
                }
                /*bool res = true;
                for (int i = 0; i < taille; ++i) {
                    for (int j = 0; j < taille; ++j) {
                        if(temp[i][j] != filter[i][j]) {
                            res = false;
                        }
                    }
                }*/
            }
        }
    }
    return imageResult;
}

cv::Mat dilation(cv::Mat o1, bool seuil) {
    cv::Mat grayFrame1 = o1.clone();
    cv::Mat imageResult = o1.clone();
    if(seuil) {
        grayFrame1 = seuillage(o1.clone());
        imageResult = seuillage(o1.clone());
    }
    uchar filter[3][3] = {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}};
    uchar temp[3][3] = {};
    for (int i = 1; i < grayFrame1.rows - 2; ++i) {
        for (int j = 1; j < grayFrame1.cols - 2; ++j) {
            temp[0][0] = grayFrame1.at<uchar>(i-1,j-1);
            temp[0][1] = grayFrame1.at<uchar>(i-1,j);
            temp[0][2] = grayFrame1.at<uchar>(i-1,j+1);

            temp[1][0] = grayFrame1.at<uchar>(i,j-1);
            temp[1][1] = grayFrame1.at<uchar>(i,j);
            temp[1][2] = grayFrame1.at<uchar>(i,j+1);

            temp[2][0] = grayFrame1.at<uchar>(i+1,j-1);
            temp[2][1] = grayFrame1.at<uchar>(i+1,j);
            temp[2][2] = grayFrame1.at<uchar>(i+1,j+1);

            bool res = false;
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    if(temp[i][j] == filter[i][j]) {
                        res = true;
                    }
                }
            }
            if(res) {
                imageResult.at<uchar>(i,j) = 255;
            } else {
                imageResult.at<uchar>(i,j) = 0;
            }
        }
    }
    return imageResult;
}

cv::Mat ouverture(cv::Mat o1, bool seuil) {
    cv::Mat gray = o1.clone();
    if(seuil) {
        gray = seuillage(o1.clone());
    }
    cv::Mat ero = erosion(gray, false, 1);
    cv::Mat res = dilation(ero, false);
    return res;
}

cv::Mat fermeture(cv::Mat o1) {
    cv::Mat gray = seuillage(o1);
    cv::Mat dila = dilation(gray, false);
    cv::Mat res = erosion(dila, false);
    return res;
}

int countBool(bool tab[8]) {
    int res = 0;
    for (int i = 0; i < 8; ++i) {
        if(tab[i]) {
            res++;
        }
    }
    return res;
}

cv::Mat amincissementtest(cv::Mat o1, bool seuil) {
    cv::Mat grayFrame1 = o1.clone();
    if(seuil) {
        grayFrame1 = seuillage(o1.clone());
    }


    cv::Mat tempMat = grayFrame1.clone();
    bool imageResult[grayFrame1.rows][grayFrame1.cols];
    for (int n = 0; n < grayFrame1.rows; ++n) {
        for (int i = 0; i < grayFrame1.cols; ++i) {
            imageResult[n][i] = false;
        }
    }
    uchar filter1[3][3] = {{255, 255, 255}, {127, 255, 127}, {0, 0, 0}};
    uchar filter2[3][3] = {{255, 255, 127}, {255, 255, 0}, {127, 0, 0}};
    uchar filter3[3][3] = {{255, 127, 0}, {255, 255, 0}, {255, 127, 0}};
    uchar filter4[3][3] = {{127, 0, 0}, {255, 255, 0}, {255, 255, 127}};
    uchar filter5[3][3] = {{0, 0, 0}, {127, 255, 127}, {255, 255, 255}};
    uchar filter6[3][3] = {{0, 0, 127}, {0, 255, 255}, {127, 255, 255}};
    uchar filter7[3][3] = {{0, 127, 255}, {0, 255, 255}, {0, 127, 255}};
    uchar filter8[3][3] = {{127, 255, 255}, {0, 255, 255}, {0, 0, 127}};
    uchar (*filter)[3] = filter1;
    uchar a = filter[2][2];
    uchar temp[3][3] = {};
    for (int k = 1; k < 9; ++k) {
        for (int i = 1; i < grayFrame1.rows - 2; ++i) {
            for (int j = 1; j < grayFrame1.cols - 2; ++j) {
                temp[0][0] = grayFrame1.at<uchar>(i-1,j-1);
                temp[0][1] = grayFrame1.at<uchar>(i-1,j);
                temp[0][2] = grayFrame1.at<uchar>(i-1,j+1);

                temp[1][0] = grayFrame1.at<uchar>(i,j-1);
                temp[1][1] = grayFrame1.at<uchar>(i,j);
                temp[1][2] = grayFrame1.at<uchar>(i,j+1);

                temp[2][0] = grayFrame1.at<uchar>(i+1,j-1);
                temp[2][1] = grayFrame1.at<uchar>(i+1,j);
                temp[2][2] = grayFrame1.at<uchar>(i+1,j+1);

                bool res = true;
                for (int i = 0; i < 3; ++i) {
                    for (int j = 0; j < 3; ++j) {
                        if((temp[i][j] != filter[i][j]) && (filter[i][j] != 127)) {
                            res = false;
                        }
                    }
                }
                if(res) {
                    imageResult[i][j] = res;
                } else {
                }
            }
        }
        if (k == 1) {
            filter = filter2;
        } else if (k == 2) {
            filter = filter3;
        } else if (k == 3) {
            filter = filter4;
        } else if (k == 4) {
            filter = filter5;
        } else if (k == 5) {
            filter = filter6;
        } else if (k == 6) {
            filter = filter7;
        } else if (k == 7) {
            filter = filter8;
        }

        for (int l = 0; l < grayFrame1.rows; ++l) {
            for (int m = 0; m < grayFrame1.cols; ++m) {
                //tempMat.at<uchar>(l,m) = (tempMat.at<uchar>(l,m) + imageResult.at<uchar>(l,m))%255;
                if(imageResult[l][m]) {
                    tempMat.at<uchar>(l,m) = 0;
                }
            }
        }
    }
    return tempMat;
}

cv::Mat amincissement(cv::Mat o1, bool seuil) {
    cv::Mat grayFrame1 = o1.clone();
    cv::Mat imageResult = o1.clone();
    for (int n = 0; n < imageResult.rows; ++n) {
        for (int i = 0; i < imageResult.cols; ++i) {
            imageResult.at<uchar>(n,i) = 0;
        }
    }
    if(seuil) {
        grayFrame1 = seuillage(o1.clone());
    }
    uchar filter1[3][3] = {{255, 255, 255},
                           {127, 255, 127},
                           {0, 0, 0}};
    uchar filter2[3][3] = {{255, 255, 127},
                           {255, 255, 0},
                           {127, 0, 0}};
    uchar filter3[3][3] = {{255, 127, 0},
                           {255, 255, 0},
                           {255, 127, 0}};
    uchar filter4[3][3] = {{127, 0, 0},
                           {255, 255, 0},
                           {255, 255, 127}};
    uchar filter5[3][3] = {{0, 0, 0},
                           {127, 255, 127},
                           {255, 255, 255}};
    uchar filter6[3][3] = {{0, 0, 127},
                           {0, 255, 255},
                           {127, 255, 255}};
    uchar filter7[3][3] = {{0, 127, 255},
                           {0, 255, 255},
                           {0, 127, 255}};
    uchar filter8[3][3] = {{127, 255, 255},
                           {0, 255, 255},
                           {0, 0, 127}};
    uchar (*filter)[3] = filter1;
    uchar a = filter[2][2];
    uchar temp[3][3] = {};
    for (int k = 1; k < 9; k++) {
        for (int i = 1; i < grayFrame1.rows - 2; ++i) {
            for (int j = 1; j < grayFrame1.cols - 2; ++j) {
                if(grayFrame1.at<uchar>(i,j) == 255) {
                    temp[0][0] = grayFrame1.at<uchar>(i-1,j-1);
                    temp[0][1] = grayFrame1.at<uchar>(i-1,j);
                    temp[0][2] = grayFrame1.at<uchar>(i-1,j+1);

                    temp[1][0] = grayFrame1.at<uchar>(i,j-1);
                    temp[1][1] = grayFrame1.at<uchar>(i,j);
                    temp[1][2] = grayFrame1.at<uchar>(i,j+1);

                    temp[2][0] = grayFrame1.at<uchar>(i+1,j-1);
                    temp[2][1] = grayFrame1.at<uchar>(i+1,j);
                    temp[2][2] = grayFrame1.at<uchar>(i+1,j+1);

                    bool res = true;
                    for (int i = 0; i < 3; ++i) {
                        for (int j = 0; j < 3; ++j) {
                            a = filter[i][j];
                            if((temp[i][j] != filter[i][j]) && (filter[i][j] != 127)) {
                                res = false;
                            }
                        }
                    }
                    if(res) {
                        imageResult.at<uchar>(i,j) = 255;
                    }
                }
            }
        }
        for (int l = 0; l < imageResult.rows; ++l) {
            for (int m = 0; m < imageResult.cols; ++m) {
                if(imageResult.at<uchar>(l,m) == 255) {
                    grayFrame1.at<uchar>(l,m) = 0;
                    imageResult.at<uchar>(l,m) = 0;
                }
            }
        }
        //imageResult = seuillage(o1.clone());
        if (k == 1) {
            filter = filter2;
        } else if (k == 2) {
            filter = filter3;
        } else if (k == 3) {
            filter = filter4;
        } else if (k == 4) {
            filter = filter5;
        } else if (k == 5) {
            filter = filter6;
        } else if (k == 6) {
            filter = filter7;
        } else if (k == 7) {
            filter = filter8;
        }
    }
    return grayFrame1;
}

cv::Mat epaississement(cv::Mat o1, bool seuil) {
    cv::Mat grayFrame1 = o1.clone();
    cv::Mat imageResult = o1.clone();
    cv::Mat tempMat = cv::Mat(o1.rows, o1.cols, CV_8UC1);
    if(seuil) {
        grayFrame1 = seuillage(o1.clone());
        imageResult = seuillage(o1.clone());
    }
    uchar filter1[3][3] = {{255, 255, 255}, {127, 0, 127}, {0, 0, 0}};
    uchar filter2[3][3] = {{255, 255, 127}, {255, 0, 0}, {127, 0, 0}};
    uchar filter3[3][3] = {{255, 127, 0}, {255, 0, 0}, {255, 127, 0}};
    uchar filter4[3][3] = {{127, 0, 0}, {255, 0, 0}, {255, 255, 127}};
    uchar filter5[3][3] = {{0, 0, 0}, {127, 0, 127}, {255, 255, 255}};
    uchar filter6[3][3] = {{0, 0, 127}, {0, 0, 255}, {127, 255, 255}};
    uchar filter7[3][3] = {{0, 127, 255}, {0, 0, 255}, {0, 127, 255}};
    uchar filter8[3][3] = {{127, 255, 255}, {0, 0, 255}, {0, 0, 127}};
    uchar (*filter)[3] = filter1;
    uchar a = filter[2][2];
    uchar temp[3][3] = {};
    for (int k = 1; k < 9; ++k) {
        for (int i = 1; i < grayFrame1.rows - 2; ++i) {
            for (int j = 1; j < grayFrame1.cols - 2; ++j) {
                temp[0][0] = grayFrame1.at<uchar>(i-1,j-1);
                temp[0][1] = grayFrame1.at<uchar>(i-1,j);
                temp[0][2] = grayFrame1.at<uchar>(i-1,j+1);

                temp[1][0] = grayFrame1.at<uchar>(i,j-1);
                temp[1][1] = grayFrame1.at<uchar>(i,j);
                temp[1][2] = grayFrame1.at<uchar>(i,j+1);

                temp[2][0] = grayFrame1.at<uchar>(i+1,j-1);
                temp[2][1] = grayFrame1.at<uchar>(i+1,j);
                temp[2][2] = grayFrame1.at<uchar>(i+1,j+1);

                bool res = true;
                for (int i = 0; i < 3; ++i) {
                    for (int j = 0; j < 3; ++j) {
                        if((temp[i][j] != filter[i][j]) && (filter[i][j] != 127)) {
                            res = false;
                        }
                    }
                }
                if(res) {
                    imageResult.at<uchar>(i,j) = 255;
                } else {
                }
            }
        }
        for (int l = 0; l < imageResult.rows; ++l) {
            for (int m = 0; m < imageResult.cols; ++m) {
                //tempMat.at<uchar>(l,m) = (tempMat.at<uchar>(l,m) + imageResult.at<uchar>(l,m))%255;
                if(imageResult.at<uchar>(l,m) == 255) {
                    tempMat.at<uchar>(l,m) = 255;
                }
            }
        }
        imageResult = grayFrame1.clone();
        if (k == 1) {
            filter = filter2;
        } else if (k == 2) {
            filter = filter3;
        } else if (k == 3) {
            filter = filter4;
        } else if (k == 4) {
            filter = filter5;
        } else if (k == 5) {
            filter = filter6;
        } else if (k == 6) {
            filter = filter7;
        } else if (k == 7) {
            filter = filter8;
        }
    }
    return tempMat;
}

int calculSeuil(cv::Mat o1) {
   float res = 0;
   int size = o1.rows * o1.cols;
   int seuil = 0;
   float var_max = 0;
   int sum = 0;
   int sumB = 0;
   int q1 = 0;
   int q2 = 0;
   double u1 = 0;
   double u2 = 0;
   int histo[255];
   double variance;
   for (int i = 0; i < 255; ++i) {
       histo[i] = 0;
   }
   for (int j = 0; j < o1.rows; ++j) {
       for (int i = 0; i < o1.cols; ++i) {
           int val = o1.at<uchar>(j,i);
           histo[val] += 1;
       }
   }
    for (int k = 0; k < 255; ++k) {
        sum += k * histo[k];
    }
    for (int l = 0; l < 255; ++l) {
        q1 += histo[l];
        if(q1 != 0 && q1 != size) {
            q2 = size - q1;
            sumB += l * histo[l];
            u1 = (double)sumB / q1;
            u2 = (double)(sum - sumB) / q2;
            variance = (double)q1*q2*(pow(u1-u2,2));
            if(variance > var_max) {
                seuil = l;
                var_max = variance;
            }
        }
    }
   return seuil;
}

void rotateMatrix(uchar mat[][N])
{
    // Consider all squares one by one
    for (int x = 0; x < N / 2; x++)
    {
        // Consider elements in group of 4 in
        // current square
        for (int y = x; y < N-x-1; y++)
        {
            // store current cell in temp variable
            uchar temp = mat[x][y];

            // move values from right to top
            mat[x][y] = mat[y][N-1-x];

            // move values from bottom to right
            mat[y][N-1-x] = mat[N-1-x][N-1-y];

            // move values from left to bottom
            mat[N-1-x][N-1-y] = mat[N-1-y][x];

            // assign temp to left
            mat[N-1-y][x] = temp;
        }
    }
}

bool empty(cv::Mat o1) {
    bool res = true;
    for (int i = 0; i < o1.rows ; ++i) {
        for (int j = 0; j < o1.cols ; ++j) {
            if (o1.at<uchar>(i,j) == 255) {
                res = false;
            }
        }
    }
    return res;
}

cv::Mat lantuejoul(cv::Mat o1, bool seuil) {
    cv::Mat grayFrame1 = o1.clone();
    cv::Mat imageResult;
    if(seuil) {
        grayFrame1 = seuillage(o1.clone());
    }
    cv::Mat sumFrame = grayFrame1.clone();
    for (int i = 0; i < o1.rows ; ++i) {
        for (int j = 0; j < o1.cols ; ++j) {
            sumFrame.at<uchar>(i,j) = 0;
        }
    }
    int t = 0;
    cv::Mat eroN = erosion(grayFrame1, false, 0);
    while (!empty(eroN)) {
        cv::Mat ouv = ouverture(eroN, false);
        imageResult = soustractionBinaire(eroN, ouv, false);
        t += 1;
        sumFrame = additionBinaire(sumFrame, imageResult, false);
        eroN = erosion(eroN, false, 1);
    }
    return sumFrame;
}