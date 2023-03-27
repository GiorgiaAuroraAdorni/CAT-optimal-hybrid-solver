
int NOTCOLOR = 5;
std::vector<int> vectorize(const std::vector<std::vector<int>>& matrix) {
    std::vector<int> result;
    for (const auto& row : matrix) {
        for (int element : row) {
            result.push_back((element > 0 ) ? 1 : 0);
        }
    }
    return result;
}

double euclideanDistance(const std::vector<int>& vec1, const std::vector<double>& vec2) {
    double sum = 0.0;
    for (int i = 0; i < vec1.size(); ++i) {
        double diff = static_cast<double>(vec1[i]) - vec2[i];
        sum += diff * diff;
    }
    return std::sqrt(sum);
}


double squared_euclidean_distance(const std::vector<int>& point1, const std::vector<int>& point2) {
    double distance = 0.0;
    for (int i = 0; i < point1.size(); ++i) {
        distance += std::pow(point1[i] - point2[i], 2);
    }
    return distance;
}

std::vector<double> computeCentroid(const std::vector<std::vector<int>> vectors) {
    int n = vectors[0].size();
    std::vector<double> centroid(n, 0);
    for (const auto& vec : vectors) {
        for (int i = 0; i < n; ++i) {
            centroid[i] += static_cast<double>(vec[i]);
        }
    }
    for (int i = 0; i < n; ++i) {
        centroid[i] /= static_cast<double>(vectors.size());
    }
    return centroid;
}

std::vector<double> intToDouble(const std::vector<int> vec) {
    std::vector<double> result(vec.size());
    std::transform(vec.begin(), vec.end(), result.begin(), [](int val) { return static_cast<double>(val); });
    return result;
}


bool areVectorsEqual(const std::vector<double> vec1, const std::vector<double> vec2) {
    return vec1.size() == vec2.size() && std::equal(vec1.begin(), vec1.end(), vec2.begin());
}

void printVectorR(const std::vector<std::vector<int>> vec) {
    for (const auto& innerVec : vec) {
        for (const auto& num : innerVec) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }
}


void printVectorR2(const std::vector<std::vector<double>> vec) {
    for (const auto& innerVec : vec) {
        for (const auto& num : innerVec) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }
}



void writeVectorToFile(const std::vector<std::vector<std::vector<int>>>& vec) {
    std::ofstream outFile("./OutputCluster/test.txt");
    if (outFile.is_open()) {
        for (const auto& outerVec : vec) {
            for (const auto& innerVec : outerVec) {
                for (const auto& val : innerVec) {
                    outFile << val << " ";
                }
                outFile << std::endl;
            }
            outFile << "===" << std::endl; // separatore tra i sottovettori esterni
        }
        outFile.close();
    }
}


std::vector<double> computeCentroidEuc(const std::vector<std::vector<int>>& cluster) {
    std::vector<double> centroid(cluster[0].size(), 0.0);
    for (int i = 0; i < cluster.size(); ++i) {
        for (int j = 0; j < cluster[i].size(); ++j) {
            centroid[j] += static_cast<double>(cluster[i][j]);
        }
    }
    for (int i = 0; i < centroid.size(); ++i) {
        centroid[i] /= cluster.size();
    }
    return centroid;
}


//__________________________________________________//
//__________________________________________________//
//__________________________________________________//
//__________________________________________________//
//__________________________________________________//
//_________________ K_MEANS EUCLIDI_________________//
//__________________________________________________//
//__________________________________________________//
//__________________________________________________//
//__________________________________________________//


std::vector<std::vector<int>> vectorClusters(int k_clusters, std::vector<Instruction> Moves, std::vector<std::vector<int>> & finalMat, std::vector<std::vector<int>> & voidMat,std::vector<int> value_index){
    std::vector<std::vector<int>> VectorMatrix;
    std::vector<std::vector<int>> Clusters;

    //Step 1: generate all vector for all move
    for(int i = 0; i < Moves.size(); ++i){
        auto tmpMat = voidMat;
        //auto allCopy = checkForBESTmove(Moves[i].instruction, Moves[i].len, Moves[i].pattern,  &finalMat, &tmpMat).first;
        auto allCopy = checkForCopy(0, 0,0, Moves[i].instruction, Moves[i].len, Moves[i].pattern, &finalMat, &tmpMat, value_index);
        for(int i = 0; i < allCopy.size(); ++i){
            executeInstruction(0, allCopy[i][0],allCopy[i][1], Moves[i].instruction, Moves[i].len,  Moves[i].pattern, &finalMat, &tmpMat,value_index);
        } 

        auto vectorizedMove = vectorize(tmpMat);
        VectorMatrix.push_back(vectorizedMove);
    }


    //Step 2 k-means
    //a initialize k centroid
    std::random_device rd;
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<int> distr(0, Moves.size()-1);
    std::vector<std::vector<double>> Centroids;
    std::vector<int> fakeHash(Moves.size(), 0);
    

    int first_centroid = distr(gen);
    Centroids.push_back(intToDouble(VectorMatrix[first_centroid]));
    auto oldCentroid = Centroids[0];
    while(Centroids.size() < k_clusters){
        std::vector<double> distances;
        double total_distance = 0.0;
        for(int i = 0; i < VectorMatrix.size(); ++i){
            double dist_tmp = euclideanDistance(VectorMatrix[i], oldCentroid);
            distances.push_back(dist_tmp);
            total_distance += dist_tmp;
        }

        std::discrete_distribution<> weighted_distrib(distances.begin(), distances.end());
        int idx = weighted_distrib(gen);
        Centroids.push_back(intToDouble(VectorMatrix[idx]));
        oldCentroid = Centroids[Centroids.size()-1];
    }
    
    bool flag_cluster = true;
    while(flag_cluster){

        //check for every cluster wich is the most near and add in this cluster
        std::vector<std::vector<std::vector<int>>> Clusters_tmp(k_clusters, std::vector<std::vector<int>>());
        std::vector<std::vector<int>> Clusters_idx_tmp(k_clusters, std::vector<int>());
        for(int i = 0; i < Moves.size(); ++i){
            int best_cluster = -1;
            double similarity = std::numeric_limits<double>::infinity();

            for(int j = 0; j < k_clusters; ++j){
                double sim_j_clus =  euclideanDistance(VectorMatrix[i], Centroids[j]);
                if(sim_j_clus < similarity){
                    best_cluster = j;
                    similarity = sim_j_clus;
                }
            }
            Clusters_tmp[best_cluster].push_back(VectorMatrix[i]);
            Clusters_idx_tmp[best_cluster].push_back(i);
        }

        //recompute the centroid
        bool flag_tmp = false;

        for(int j = 0; j < k_clusters; ++j){
            auto old_centroid = Centroids[j];
            auto new_centroid = computeCentroidEuc(Clusters_tmp[j]);
            Centroids[j] = new_centroid;
            flag_tmp = flag_tmp || !(areVectorsEqual(old_centroid,new_centroid));

        }
       
        flag_cluster = flag_tmp;
        if(!flag_cluster){
            writeVectorToFile(Clusters_tmp);
        }
        Clusters = Clusters_idx_tmp;
    }

    return Clusters;
}





//__________________________________________________//
//__________________________________________________//
//__________________________________________________//
//__________________________________________________//
//__________________________________________________//
//_________________ COSINE K_MEANS _________________//
//__________________________________________________//
//__________________________________________________//
//__________________________________________________//
//__________________________________________________//


double cosineSimilarity(const std::vector<int>& vec1, const std::vector<double>& vec2) {
    // Calcola la lunghezza dei vettori
    double vec1_length = 0.0;
    double vec2_length = 0.0;
    for (int i = 0; i < vec1.size(); ++i) {
        vec1_length += vec1[i] * vec1[i];
    }
    vec1_length = std::sqrt(vec1_length);
    for (int i = 0; i < vec2.size(); ++i) {
        vec2_length += vec2[i] * vec2[i];
    }
    vec2_length = std::sqrt(vec2_length);

    // Calcola il prodotto interno dei vettori
    double dot_product = 0.0;
    for (int i = 0; i < vec1.size(); ++i) {
        dot_product += vec1[i] * vec2[i];
    }

    // Calcola la similarità del coseno
    double similarity = dot_product / (vec1_length * vec2_length);
    return similarity;
}

std::vector<double> computeCentroidCos(const std::vector<std::vector<int>>& clusterPoints) {
    int numPoints = clusterPoints.size();

    int numDimensions = clusterPoints[0].size();

    std::vector<double> centroid(numDimensions, 0.0);

    for (int i = 0; i < numPoints; ++i) {
        for (int j = 0; j < numDimensions; ++j) {
            centroid[j] += static_cast<double>(clusterPoints[i][j]);
        }
    }

    for (int j = 0; j < numDimensions; ++j) {
        centroid[j] /= static_cast<double>(numPoints);
    }

    return centroid;
}

void debpr(const std::vector<double>& vec) {
    for (const auto& val : vec) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}

std::vector<std::vector<int>> K_means_cos(int k_clusters, std::vector<Instruction> Moves, std::vector<std::vector<int>> & finalMat, std::vector<std::vector<int>> & voidMat,std::vector<int> value_index){
    std::vector<std::vector<int>> VectorMatrix;
    std::vector<std::vector<int>> Clusters;

    //Step 1: generate all vector for all move
    for(int i = 0; i < Moves.size(); ++i){
        auto tmpMat = voidMat;
        auto allCopy = checkForCopy(0, 0,0, Moves[i].instruction, Moves[i].len, Moves[i].pattern, &finalMat, &tmpMat, value_index);
        for(int i = 0; i < allCopy.size(); ++i){
            executeInstruction(0, allCopy[i][0],allCopy[i][1], Moves[i].instruction, Moves[i].len,  Moves[i].pattern, &finalMat, &tmpMat,value_index);
        } 

        auto vectorizedMove = vectorize(tmpMat);
        VectorMatrix.push_back(vectorizedMove);
    }


    //Step 2 k-means
    //a initialize k centroid
    std::random_device rd;
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<int> distr(0, Moves.size()-1);
    std::vector<std::vector<double>> Centroids;
    std::vector<int> fakeHash(Moves.size(), 0);
    

    int first_centroid = distr(gen);
    Centroids.push_back(intToDouble(VectorMatrix[first_centroid]));
    int int_old_idx = first_centroid;
    while(Centroids.size() < k_clusters){
        std::vector<double> distances;
        double total_distance = 0.0;
        for(int i = 0; i < VectorMatrix.size(); ++i){
            double dist_tmp = cosineSimilarity(VectorMatrix[i], intToDouble(VectorMatrix[int_old_idx]));
            distances.push_back(dist_tmp);
            total_distance += dist_tmp;
        }

        std::discrete_distribution<> weighted_distrib(distances.begin(), distances.end());
        int idx = weighted_distrib(gen);
        Centroids.push_back(intToDouble(VectorMatrix[idx]));
        int_old_idx = idx;
    }

    
    bool flag_cluster = true;
    while(flag_cluster){

        //check for every cluster wich is the most near and add in this cluster
        std::vector<std::vector<std::vector<int>>> Clusters_tmp(k_clusters, std::vector<std::vector<int>>());
        std::vector<std::vector<int>> Clusters_idx_tmp(k_clusters, std::vector<int>());
        for(int i = 0; i < Moves.size(); ++i){
            int best_cluster = -1;
            double similarity = -100;

            for(int j = 0; j < k_clusters; ++j){
                double sim_j_clus =  cosineSimilarity(VectorMatrix[i], Centroids[j]);
                if(sim_j_clus > similarity){
                    best_cluster = j;
                    similarity = sim_j_clus;
                }
            }
            Clusters_tmp[best_cluster].push_back(VectorMatrix[i]);
            Clusters_idx_tmp[best_cluster].push_back(i);
        }

        //recompute the centroid
        bool flag_tmp = false;

        for(int j = 0; j < k_clusters; ++j){
            auto old_centroid = Centroids[j];

            auto clusters_tmp = Clusters_tmp[j];
            auto new_centroid = old_centroid;
            if(clusters_tmp.size() != 0){
                new_centroid = computeCentroidCos(clusters_tmp);
            }
            Centroids[j] = new_centroid;
            flag_tmp = flag_tmp || !(areVectorsEqual(old_centroid,new_centroid));

        }
        flag_cluster = flag_tmp;
        if(!flag_cluster){
            writeVectorToFile(Clusters_tmp);
        }
        Clusters = Clusters_idx_tmp;
    }

    return Clusters;
}
