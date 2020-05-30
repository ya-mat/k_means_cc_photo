#include <iostream>
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core/eigen.hpp>

//int main(int argc, const char* argv[])
int main(){
  cv::Mat mat = cv::imread("sample.jpg", cv::IMREAD_COLOR);

// 1チャンネルに変換し，Eigenに変換
  Eigen::MatrixXd em;
  cv::cv2eigen(mat.reshape(1, mat.rows*mat.cols), em);

// Eigen側の処理
  const int kk = 3;
  const int nn = mat.rows*mat.cols;
  std::cout << "nn " << nn << std::endl;
  em.transposeInPlace();
//  Eigen::MatrixXd mu = em.block(0, 0, 3, kk);
  Eigen::MatrixXd mu(3, kk);
  mu << 255, 0, 0, 0, 255, 0, 0, 0, 255;
//  mu << 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 50, 50, 50, 100, 100, 100;
  std::cout << "mu = " << mu << std::endl;
  Eigen::VectorXi gk(nn);
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
  std::cout << "mu =" << mu << std::endl;
  em.transposeInPlace();

// cvに戻す
  cv::Mat tmp;
  cv::eigen2cv(em, tmp);

// 3チャンネルに変換
  tmp = tmp.reshape(3, mat.rows);

  cv::namedWindow("sample", cv::WINDOW_AUTOSIZE);
  cv::imshow("sample", mat);
  cv::waitKey(0);
  cv::destroyAllWindows();

  cv::namedWindow("tmp", cv::WINDOW_AUTOSIZE);
  cv::imshow("tmp", tmp/255);
  cv::waitKey(0);
  cv::destroyAllWindows();

  return 0;
}

