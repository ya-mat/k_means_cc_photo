#include <iostream>
#include <fstream>
#include <Eigen/Dense>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/eigen.hpp>

int main(){
  cv::Mat mat = cv::imread("sample.jpg", cv::IMREAD_COLOR);
  if(mat.empty()){
    std::cout << "There is no input file 'sample.jpg'" << std::endl;
    exit(EXIT_FAILURE);
  }

  const int mr = mat.rows;
  const int mc = mat.cols;
  const int nn = mr*mc;

  if(RAND_MAX < mr){
    std::cout << "rand_max " << RAND_MAX << std::endl;
    std::cout << "mat.rows " << mr << std::endl;
    std::cout << "RAND_MAX < mr, error!" << std::endl;
  }

// 1チャンネルに変換し，Eigenに変換
  Eigen::MatrixXd em;
  cv::cv2eigen(mat.reshape(1, mr*mc), em);

// Eigen側の処理
  int kk;
  std::ifstream fr;
  fr.open("input_kk", std::ios::in);
  if(! fr.fail()){
    fr >> kk;
    std::cout << "kk = " << kk << std::endl;
    fr.close();
  }
  else{
    std::cout << "There is no input file 'input_kk'" << std::endl;
    exit(EXIT_FAILURE);
  }
  Eigen::VectorXi gk(nn);
  Eigen::VectorXi numk = Eigen::VectorXi::Zero(kk);
  Eigen::MatrixXd::Index minIndex;
  int i;
  int j;
  int l;
  int sum_gk;
  int bef_sum_gk;

  std::cout << "nn " << nn << std::endl;
  em.transposeInPlace();
  Eigen::MatrixXd mu(3, kk);
  for(i = 0; i < kk; i++){
    mu.col(i) = em.col((rand() % mr));
  }
//  std::cout << "mu = " << mu << std::endl; //dbg

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
      if(numk(l) != 0){
	mu.col(l) /= static_cast<double> (numk(l));
      }
      else{
	mu.col(l) = em.col((rand() % mr));
      }
    }
    sum_gk = gk.sum();
    if(bef_sum_gk == sum_gk) break;
    bef_sum_gk = sum_gk;
//    std::cout << "numk =" << numk << std::endl; //dbg
//    std::cout << "mu =" << mu << std::endl; //dbg
  }

  //色を減らす
  for(l = 0; l < kk; l++){
    for(j = 0; j < nn; j++){
      if(gk(j) == l){
	em.col(j) = mu.col(l);
      }
    }
  }
//  std::cout << "mu =" << mu << std::endl; //dbg
  em.transposeInPlace();

// cvに戻す
  cv::Mat tmp;
  cv::eigen2cv(em, tmp);

// 3チャンネルに変換
  tmp = tmp.reshape(3, mr);

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

