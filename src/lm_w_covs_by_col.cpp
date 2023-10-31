#include <fstream>
#include <iostream>
#include <RcppArmadillo.h>

using namespace Rcpp;

// [[Rcpp::export]]
List LM_w_COVS_by_col(const arma::vec y, const arma::mat X, const arma::mat COVS, double sigma2) {
  
  int n = X.n_rows, d = X.n_cols, p = COVS.n_cols;
  
  arma::mat coef_mat(d,2+p);
  arma::mat se(d,2+p);
  arma::mat X1(n,1,arma::fill::ones);
  
  for(int col=0;col<d; ++col){
    
    arma::mat X2=arma::join_rows(X1,X.col(col),COVS);
    
    arma::colvec coef = arma::solve(X2, y); 
    arma::colvec resid = y - X2*coef; 
    
    arma::colvec stderrest = 
      arma::sqrt(sigma2 * arma::diagvec( arma::inv(arma::trans(X2)*X2)) );
    
    coef_mat.row(col) = arma::trans(coef);
    se.row(col) = arma::trans(stderrest);
  }
  
  return List::create(Named("Coefficients") = coef_mat, Named("StdErr") = se);
}







