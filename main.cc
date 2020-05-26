#include <iostream>
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core/eigen.hpp>

//int main(int argc, const char* argv[])
int main()
{
  cv::Mat mat = cv::imread("sample.jpg", cv::IMREAD_COLOR);
//  cv::Mat mat = cv::imread("sample.jpg", CV_8UC1);

//  std::cout << "jpg " << mat << std::endl;

// 1チャンネルに変換
  cv::Mat onech = mat.reshape(1, mat.rows*mat.cols);
//  std::cout << "jpg " << onech << std::endl;

// Eigenに変換
  Eigen::MatrixXd em;
  cv::cv2eigen(onech, em);
//  cv::cv2eigen(mat, em);

// Eigen側の処理
  const int kk = 3;
  const int nn = mat.rows*mat.cols;
  std::cout << "nn " << nn << std::endl;
  em.transposeInPlace();
//  std::cout << "eigen jpg " << em << std::endl;
//  Eigen::MatrixXd mu = em.block(0, 0, 3, kk);
  Eigen::MatrixXd mu(3, kk);
//  mu << 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 50, 50, 50, 100, 100, 100;
  mu << 255, 0, 0, 0, 255, 0, 0, 0, 255;
  std::cout << "mu = " << mu << std::endl;
  Eigen::VectorXi gk = Eigen::VectorXi(nn);
  Eigen::VectorXi numk = Eigen::VectorXi::Zero(kk);
  Eigen::MatrixXd::Index minIndex;
  int i;
  int j;
  int l;
  int sum_gk;
  int bef_sum_gk;

  bef_sum_gk = 0;
  for(i = 0; i < 100; i++){
    std::cout << "loop =" << i << std::endl;
    for(j = 0; j < nn; j++){
      (mu.colwise() - em.col(j)).colwise().squaredNorm().minCoeff(&minIndex);
      gk(j) = minIndex;
    }
    numk.array() = 0;
    for(l = 0; l < kk; l++){
      mu.col(l).array() = 0.0;
      for(j = 0; j < nn; j++){
	if(gk(j) == l){
	  numk(l) += 1;
	  mu.col(l) += em.col(j);
	}
      }
      mu.col(l) /= static_cast<double> (numk(l));
    }
    sum_gk = gk.sum();
    if(bef_sum_gk == sum_gk) break;
    bef_sum_gk = sum_gk;
    std::cout << "numk =" << numk << std::endl; //dbg
    std::cout << "mu =" << mu << std::endl; //dbg
  }

  //色を減らす
  for(l = 0; l < kk; l++){
    for(j = 0; j < nn; j++){
      if(gk(j) == l){
	em.col(j) = mu.col(l);
      }
    }
  }
//  std::cout << "eigen jpg " << em << std::endl;
//  std::cout << "gk =" << gk << std::endl;
  std::cout << "mu =" << mu << std::endl;
  em.transposeInPlace();

//  em = em.cast <int> ();

//  Eigen::MatrixXi emi = ;
//  std::cout << "eigen jpg " << em << std::endl;

// cvに戻す
  cv::Mat tmp;
//  cv::Mat tmp(mat.rows, mat.cols, CV_8UC1);
//  cv::Mat tmp(mat.size(), mat.type());
  cv::eigen2cv(em, tmp);
//  cv::eigen2cv(emi, tmp);

// 3チャンネルに変換
  onech = onech.reshape(3, mat.rows);
  tmp = tmp.reshape(3, mat.rows);

//  std::cout << "tmp =" << tmp << std::endl;
  std::cout << "mat.depth() = " << mat.depth() << std::endl;
  std::cout << "tmp.depth() = " << tmp.depth() << std::endl;

  cv::namedWindow("sample", cv::WINDOW_AUTOSIZE);
  cv::imshow("sample", mat);
  cv::waitKey(0);
  cv::destroyAllWindows();

  cv::namedWindow("onech", cv::WINDOW_AUTOSIZE);
  cv::imshow("onech", onech);
  cv::waitKey(0);
  cv::destroyAllWindows();

  cv::namedWindow("tmp", cv::WINDOW_AUTOSIZE);
  cv::imshow("tmp", tmp/255);
  cv::waitKey(0);
  cv::destroyAllWindows();

  return 0;
}


/*
#include <iostream>
#include <Eigen/Dense>

int main()
{
  Eigen::MatrixXd xn;
  Eigen::MatrixXd mu;
  Eigen::VectorXi gk;
  Eigen::VectorXi numk;
  Eigen::MatrixXd::Index minIndex;
  int ni;
  int nn;
  int kk;
  int i;
  int j;
  int l;
  int sum_gk;
  int bef_sum_gk;

  // after :: from file input
  ni = 10;
  kk = 3;

  nn = ni*kk;
  std::cout << "nn " <<  nn << std::endl;

  xn = Eigen::MatrixXd::Random(2, nn);
  gk = Eigen::VectorXi(nn);
  numk = Eigen::VectorXi::Zero(kk);
  std::cout << "xn =" << std::endl << xn << std::endl;

  xn.block(0, nn/kk, 1, nn/kk).array() += 2.0;
  xn.block(0, 2*nn/kk, 1, nn/kk).array() += 4.0;

  mu = xn.block(0, 0, 2, kk);

  std::cout << "mu =" << std::endl << mu << std::endl;

  bef_sum_gk = 0;
  for(i = 0; i < 100; i++){
    std::cout << "loop =" << i << std::endl;
    for(j = 0; j < nn; j++){
      (mu.colwise() - xn.col(j)).colwise().squaredNorm().minCoeff(&minIndex);
      gk(j) = minIndex;
    }
    numk.array() = 0;
    for(l = 0; l < kk; l++){
      mu.col(l).array() = 0.0;
      for(j = 0; j < nn; j++){
	if(gk(j) == l){
	  numk(l) += 1;
	  mu.col(l) += xn.col(j);
	}
      }
      mu.col(l) /= static_cast<double> (numk(l));
    }
    sum_gk = gk.sum();
    if(bef_sum_gk == sum_gk) break;
    bef_sum_gk = sum_gk;
  }

  std::cout << "numk =" << std::endl << numk << std::endl;
  std::cout << "gk =" << std::endl << gk << std::endl;
  std::cout << "mu =" << std::endl << mu << std::endl;
}
*/
