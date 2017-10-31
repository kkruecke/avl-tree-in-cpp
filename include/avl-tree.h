#ifndef avl-tree_h_
#define avl-tree_h_

#include <memory>
#include <utility>

// fwd declarations
template<typename Key, typename Value> class avltree;    
template<typename Key, typename Value> class Node; 

template<class Key, class Value>
class avltree {

    // A union is used to hold to two types of pairs, one has a non-const Key; the other has a const Key.
    // This eliminates the constant need to cast: const_cast<Key>(p.first) = some_noconst_key,
    // while still allowing us to return values of type 'std::pair<const Key, Value>&' from avltree methods.                                   
    
   union KeyValue { 
       friend class avltree<Key, Value>;
   
       std::pair<Key, Value>        _pair;  // ...this eliminates constantly casting of const_cast<Key>(p.first) = some_noconst_key;
       std::pair<const Key, Value>  _constkey_pair;  // but always return this member of the union.

       KeyValue() {} 
       KeyValue(Key key, const Value& value) : _pair{key, value} {}
       
       KeyValue(const KeyValue& lhs) : _pair{lhs._pair.first, lhs._pair.second} {}
       
       KeyValue(Key k, Value&& v) : _pair{k, std::move(v)} {} 
   
       KeyValue(KeyValue&& lhs) :  _pair{move(lhs._pair)} {}
   
       KeyValue& operator=(const KeyValue& lhs) noexcept;  
       KeyValue& operator=(KeyValue&& lhs) noexcept; 

       constexpr Key&  key()  { return _pair.first; }
       constexpr const Key& key() const { return _constkey_pair.first; }

       constexpr Value&  value()  { return _pair.second; }
       constexpr const Value& value() const { return _constkey_pair.second; }

     public:    
       constexpr const std::pair<Key, Value>& pair() const { return _pair; }
       constexpr std::pair<Key, Value>& pair() { return _pair; }
        
       constexpr const std::pair<const Key, Value>& constkey_pair() const { return _constkey_pair; }
       constexpr       std::pair<const Key, Value>& constkey_pair() { return _constkey_pair; }

       friend std::ostream& operator<<(std::ostream& ostr, const KeyValue& key_value)
       {
          ostr << "{" << key_value._pair.first << ',' <<  key_value._pair.second <<  "}, ";
          return ostr;
       }
   };
  
   class Node { // public nested node class Tree<Key, Value>::Node
     private:  
       friend class avltree<Key, Value>;             

       Node *parent; /* parent is only used for navigation of the tree. It does not own the memory it points to. */
    
       KeyValue keys_value; 
       
       unsigned int     height;

       std::shared_ptr<Node> left;
       std::shared_ptr<Node> right;
       
       constexpr Node *getParent() noexcept; 
   
       void connectChild(int childNum, std::shared_ptr<Node>& child) noexcept;
       /*
        * Removes child node (implictly using move ctor) and shifts its children to fill the gap. Returns child pointer.
        */  
       std::unique_ptr<Node> disconnectChild(int child_index) noexcept; 
    
       public:
             
           Node() noexcept;

           explicit Node(Key key, const Value& value, Node *parent=nullptr) noexcept;

           explicit Node(const KeyValue& key_value, Node *parent=nullptr) noexcept;

           explicit Node(const KeyValue&, const KeyValue&, Node *parent=nullptr) noexcept;
           
           explicit Node(KeyValue&& key_value) noexcept; 
           
           constexpr const Node *getParent() const noexcept;

           constexpr Key& key() { return keys_value.key(); } 

           constexpr Value& value() { return keys_value.value(); } 
    
           std::ostream& print(std::ostream& ostr) const noexcept;
   
           friend std::ostream& operator<<(std::ostream& ostr, const Node& node234)
           { 
             return node.print(ostr);
           }
   }; // end class avltree<Key, Value>::Node  

   shared_ptr<Node> root;

   public:
};

#endif
