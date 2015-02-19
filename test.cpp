
//int main()  {
//    
//    deque<int> NBT;
//    Distances dist(6);
//    
//    for (int i = 0; i < 6; i++) {
//        NBT.push_back(i);
//        for (int j = 0; j < i; j++) {
//            dist.set(i, j, 0);
//        }
//    }
//    
//    dist.set(0, 1, 9);
//    dist.set(0, 4, 3);
//    dist.set(0, 5, 5);
//    dist.set(1, 2, 5);
//    dist.set(1, 5, 4);
//    dist.set(2, 3, 2);
//    dist.set(2, 5, 8);
//    dist.set(3, 4, 1);
//    dist.set(3, 4, 7);
//    dist.set(4, 5, 5);
//    
//    Edge e;
//    
//    e = prims(NBT.begin(), NBT.end(), dist, 0, 0);
//    
//    cout << e.distance + dist.at(0, e.end) << "\n";
//    
//    return 0;
//}