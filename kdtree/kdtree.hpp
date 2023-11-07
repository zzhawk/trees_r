// Copyright 2023 watson.wang
// https://github.com/zzhawk 
// Simplified from  https://blog.csdn.net/weixin_45867382/article/details/121731422

#include <algorithm>
#include <vector>

template<typename coordinate_type, size_t dimensions>
struct point {
   point(std::vector<coordinate_type> val): val(val){}
   point(std::initializer_list<coordinate_type> list) {
      val.resize(dimensions);
      size_t n = std::min(dimensions, list.size());
      std::copy_n(list.begin(), n, val.begin());
   }


   std::vector<coordinate_type> val;

   double distance(const point& pt) const {
      double dist = 0;
      for (size_t i = 0; i < dimensions; ++i) {
         double d = val.at(i) - pt.val.at(i);
         dist += d * d;
      }
      return dist;
   }
};

template<typename coordinate_type, size_t dimensions>
class kdtree {
public:
   typedef point<coordinate_type, dimensions> point_type;

private:
   struct node {
      node(const point_type& pt) : pt(pt){}
      point_type pt;
      node* left{};
      node* right{};
   };

public:

   template <typename iterator>
   explicit kdtree(iterator begin, iterator end) : _nodes(begin, end) {
      _root = build_tree(0, _nodes.size(), 0);
   }

   virtual ~kdtree() = default;

   kdtree(const kdtree& ) = default;
   kdtree(kdtree&& ) noexcept = default;

   kdtree & operator = (const kdtree&) = default;
   kdtree & operator = (kdtree&&) noexcept = default;

   const point_type& find_nearest(const point_type& pt) {
      if (_root == nullptr)
         throw std::logic_error("tree is empty");
      _best = nullptr;
      _best_dist = 0;
      nearest(_root, pt, 0);
      return _best->pt;
   }

private:

   node* _best{};
   node* _root{};
   double _best_dist{};
   std::vector<node> _nodes;

   node* build_tree(size_t begin, size_t end, size_t index) {
      if (end <= begin)
         return nullptr;
      size_t n = begin + (end - begin) / 2;
      auto i = _nodes.begin();
      std::nth_element(i + begin, i + n, i + end, [index](const node& n1, const node& n2) -> bool {
         return n1.pt.val.at(index) < n2.pt.val.at(index);
         });
      index = (index + 1) % dimensions;
      _nodes[n].left = build_tree(begin, n, index);
      _nodes[n].right = build_tree(n + 1, end, index);
      return &_nodes[n];
   }

   void nearest(node* root, const point_type& pt, size_t index) {
      if (root == nullptr)
         return;

      double d = root->pt.distance(pt);
      if (_best == nullptr || d < _best_dist) {
         _best_dist = d;
         _best = root;
      }
      if (_best_dist == 0)
         return;
      double dx = root->pt.val.at(index) - pt.val.at(index);
      index = (index + 1) % dimensions;
      nearest(dx > 0 ? root->left : root->right, pt, index);
      if (dx * dx >= _best_dist)
         return;
      nearest(dx > 0 ? root->right : root->left, pt, index);
   }
};