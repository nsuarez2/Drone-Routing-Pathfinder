#include "main.h"

using namespace std;

int main(int argc, char ** argv) {
    std::ios_base::sync_with_stdio(false);
    
    char clientType = '\0';
    
    int opt = 0, index = 0;
    while((opt = getopt_long (argc, argv, "c:h", longOpts, &index)) != -1) {
        switch(opt) {
            case 'c':
                clientType = *optarg;
                if (clientType != 'A' &&
                    clientType != 'B' &&
                    clientType != 'C') {
                    cerr << "CLIENT_TYPE is required and must be exactly A, B, "
                    << "or C" << endl;
                    exit(1);
                }
                break;
            case 'h': {
                cout << "The flag '-c' or '--clientType' is required and has a "
                     << "required argument.\nThe argument must be either 'A', '"
                     << "B', or 'C'." << endl;
                exit(0);
            }
        }
    }
    // FOR XCODE TESTING
    ifstream arq(getenv("sample4"));
    cin.rdbuf(arq.rdbuf());
    
    ostringstream os;
    
    vector<Vertex> facil;
    Vertex facility;
    string trash;
    double total_distance = 0;
    
    cin >> trash >> numFacil;
    facil.reserve(numFacil + 1);
    
    if (clientType == 'A') {
        
        deque<Subset> subs;
        Subset subset;
        
        for (int i = 0; i < numFacil; i++) {
            cin >> facility.x >> facility.y;
            facil[i] = facility;
            subset.parent = i;
            subs.push_back(subset);
        }
        
        cin >> trash >> numFacil; // really numEdges
        
        priority_queue<Edge*, deque<Edge>, shortDisComp> edges;
        Edge path;

        for (int i = 0; i < numFacil; i++) {
            cin >> path.begin >> path.end;
            path.distance = Distance(facil[path.begin], facil[path.end]);
            edges.push(path);
        }
        
        int same = 0, branch = 0;
        
        while (!edges.empty()) {
            
            Edge here = edges.top();
            edges.pop();
            
            same = findRoot(subs, here.begin);
            branch = findRoot(subs, here.end);
            
            if (same != branch) {
                
                os << here;
                
                unite(subs, same, branch);
                
                total_distance += here.distance;
            }
        }
    }
    
    if (clientType == 'B') {
        
        // indexs for Prim's
        unordered_map<int, int> notBeenTo(numFacil);
        vector<double> prim(numFacil, DOUBLEMAX);
        
        for (int i = 0; i < numFacil; i++) {
            cin >> facility.x >> facility.y;
            facil[i] = facility;
            notBeenTo[i] = 0;
        }
        
        int here = 0, next = 0;
        prim[here] = 0;
        notBeenTo.erase(here);
        
        while (!notBeenTo.empty()) {
            
            double min = DOUBLEMAX;
            double tempDist;
            
            for (auto i: notBeenTo) {
                double &thisPrim = prim[i.first];
                tempDist = Distance(facil[here], facil[i.first]);
                
                if (tempDist < thisPrim) {
                    thisPrim = tempDist;
                    notBeenTo[i.first] = here;
                }
                // for selecting next facility
                if (thisPrim < min) {
                    min = thisPrim;
                    next = i.first;
                }
            }
            
            // assign next facility
            here = next;
            
            total_distance += prim[here];
            
            if (here < notBeenTo[here])
                os << here << " " << notBeenTo[here] << "\n";
            else
                os << notBeenTo[here] << " " << here << "\n";
            
             notBeenTo.erase(here);
        }
        
    }
    
    if (clientType == 'C') {
        
        deque<int> notBeenTo;
        Path path;
        Path best;
        vector<double> dist(numFacil * numFacil + 1, 0);
        double weight;
        
        for (int i = 0; i < numFacil; i++) {
            cin >> facility.x >> facility.y;
            facil[i] = facility;
            notBeenTo.push_back(i);
            for (int j = 0; j < i; j++) {
                weight = Distance(facil[i],facil[j]);
                dist[i * numFacil + j] = weight;
                dist[j * numFacil + i] = weight;
            }
        }
        
        path.push(notBeenTo.front());
        best.push(notBeenTo.front());
        
        if (notBeenTo.size() > 2)
            upperbound(notBeenTo.begin(), notBeenTo.end(), dist, best);
        else
            best.cost = DOUBLEMAX;
        
        notBeenTo.pop_front();
        
        permute(path, notBeenTo, best, dist);
        
        // print best
        for (auto i = best.data.begin(); i != best.data.end() - 1; i++) {
            os << *i << " ";
        }
        os << best.back() << "\n";
        total_distance = best.cost;
    }
    
    cout << fixed << setprecision(2) << total_distance << '\n';
    cout << os.str();
    
    return 0;
}

double Distance(Vertex &one, Vertex &two) {
    return sqrt(pow(double(two.x - one.x), 2) + pow(double(two.y - one.y), 2));
}

int findRoot(deque<Subset> &subs, int here) {
    
    if (subs[here].parent != here)
        subs[here].parent = findRoot(subs, subs[here].parent);
    
    return subs[here].parent;
}

void unite(deque<Subset> &subs, int a, int b) {
    
    int aRoot = findRoot(subs, a);
    int bRoot = findRoot(subs, b);
    
    if (subs[a].size < subs[b].size) {
        subs[a].parent = bRoot;
        subs[b].size += subs[a].size;
    }
    else {
        subs[b].parent = aRoot;
        subs[a].size += subs[b].size;
    }
}

void permute(Path &path, deque<int> &notBeenTo, Path &best,
             vector<double> &dist) {
    
    double add;
    
    if (notBeenTo.size() > 2)
        if (lowerbound(notBeenTo.begin(), notBeenTo.end(), dist, path.back())
            + path.cost >= best.cost)
            return;
    
    if (notBeenTo.empty()) {
        add = dist[path.back()];
        path.cost += add;
//        for (auto i: path.data)
//            cout << i << " ";
//        cout << "- " << path.cost << "     " << best.cost << endl;
        
        if (path.cost < best.cost)
            best = path;
        
        path.cost -= add;
        return;
    }
    for (unsigned int i = 0; i < notBeenTo.size(); i++) {
        path.push(notBeenTo.front());
        notBeenTo.pop_front();
        add = dist[path.back() * numFacil + path.secondBack()];
        path.cost += add;
        
        permute(path, notBeenTo, best, dist);
        
        path.cost -= add;
        notBeenTo.push_back(path.back());
        path.pop();
    }
}

double lowerbound(deque<int>::iterator beg, deque<int>::iterator end,
                  vector<double> &dist, int here) {
    
    
    vector<double> prim(numFacil, DOUBLEMAX);
    unordered_set<int> notBeenTo(beg, end);
    int next = 0;
    prim[here] = 0;
    double distance = 0;
    
    notBeenTo.insert(0);
    
    while (!notBeenTo.empty()) {
        
        double min = DOUBLEMAX;
        double tempDist;
        
        for (auto i: notBeenTo) {
            double &thisPrim = prim[i];
            tempDist = dist[here * numFacil + i];
            if (tempDist < thisPrim) {
                thisPrim = tempDist;
            }
            if (thisPrim < min) {
                min = thisPrim;
                next = i;
            }
        }
        here = next;
        distance += prim[here];
        notBeenTo.erase(here);
    }
    
    return distance;
}

void upperbound(deque<int>::iterator beg, deque<int>::iterator end,
                vector<double> &dist, Path &best) {
    
    unordered_set<int> NBT(beg, end);
    
    int here = 0, next = 0;
    NBT.erase(0);
    
    for (int i = 0; i < numFacil - 1; i++) {
        
        double min = DOUBLEMAX;
        
        for (auto j: NBT) {
            
            double temp = dist[here * numFacil + j];
            
            if (temp < min){
                min = temp;
                next = j;
            }
        }
        
        best.push(next);
        best.cost += min;
        NBT.erase(next);
        here = next;
    }
    
    best.cost += dist[next];
}


