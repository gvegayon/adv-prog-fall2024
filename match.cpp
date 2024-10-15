#include <Rcpp.h>

using namespace Rcpp;

// [[Rcpp::export]]
List match_it(
    const NumericVector & X,
    const IntegerVector & group,
    const NumericVector & Y
) {


    // Identifying the ids of the groups
    std::vector< int > treated;
    std::vector< int > control;

    for (auto & g: group) {
        if (g == 1) {
            treated.push_back(g);
        } else if (g == 0) {
            control.push_back(g);
        }
    }

    // Running the matching
    IntegerMatrix matches(X.size(), 2);

    for (size_t i = 0u; i < X.size(); ++i)
    {

        // Looking for which group should we use
        auto & group_i = group[i] == 1 ? control : treated;

        // Matches
        int id_0 = group_i[0], id_1 = group_i[1];
        double left_0 = std::abs(X[i] - X[id_0]);
        double right_0 = std::abs(X[i] - X[id_1]);

        // Finding the closest match
        for (size_t j = 2u; j < group_i.size(); ++j){

            double d = X[i] - X[group_i[j]];

            if (d < 0 && std::abs(d) < left_0) {
                left_0 = std::abs(d);
                id_0 = group_i[j];
            } else if (d > 0 && d < right_0) {
                right_0 = d;
                id_1 = group_i[j];
            } 

        }

        // Saving the results
        matches(i, 0) = id_0;
        matches(i, 1) = id_1;


    }

    return List::create(
        _["X"] = X,
        _["group"] = group,
        _["Y"] = Y,
        _["matches"] = matches
    );

}

