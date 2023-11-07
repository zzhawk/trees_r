#include "binaryTreeRaw.hpp"

void onelifecycle()
{
   binaryTree<double> bt(50.0);
   bt.append(38.0);
   bt.append(60.0);
   bt.append(20.0);
   bt.append(40.0);
   bt.append(51.0);
   bt.append(62.0);
   bt.append(54.0);
   bt.append(52.0);
   bt.append(63.0);
   bt.append(55.0);

   auto look = bt.del(50.0);

}

void onelifecycle2()
{
   avl<double> bt(9.0);
   bt.append(7.0);
   bt.append(15.0);
   bt.append(6.0);
   bt.append(8.0);
   bt.append(11.0);
   bt.append(16.0);
   bt.append(12.0);
   bt.append(10.0);
   bt.append(13.0);

   bt.del(15.0);

   bt.print();
}

int main()
{
   onelifecycle2();
   return 0;
}