#include <iostream>
#include "kdtree.hpp"
// https://blog.csdn.net/weixin_45867382/article/details/121731422

int main()
{
   typedef kdtree<int, 2> tree2d;
   typedef point<int, 2> point2d;
   point2d points[] = { { 2, 3 }, { 5, 4 }, { 9, 6 }, { 4, 7 }, { 8, 1 }, { 7, 2 } };


   tree2d tree(std::begin(points), std::end(points));
   point2d n = tree.find_nearest({ 9,2 });

   //std::cout << "Wikipedia example data:\n";
   //std::cout << "nearest point: " << n << '\n';
   //std::cout << "distance: " << tree.distance() << '\n';
   //std::cout << "nodes visited: " << tree.visited() << '\n';

   return 0;
}