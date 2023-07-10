#include <iostream>
#include <vector>
#include <stack>


using namespace std;

class BinarySearchTree{
private:
    int data;
    BinarySearchTree* left {};
    BinarySearchTree* right {};
public:
    BinarySearchTree(int data):data(data){}
    BinarySearchTree(int data, BinarySearchTree*left, BinarySearchTree*right):data(data),left(left),right(right){}

    /* BinarySearchTree(deque<int> &preorder){
        deque<int> l;
        deque<int> r;
        data = preorder[0];

        preorder.pop_front();

        int i = 0;
        while(!preorder.empty()){
            if(preorder[i] < data){
                l.push_back(preorder[i]);
                preorder.pop_front();

            }
            else if(preorder[i] > data){
                r.push_back(preorder[i]);
                preorder.pop_front();

            }
            i++;
        }
        if(!l.empty()){
            left = new BinarySearchTree(l);
        }
        if(!r.empty()){
            left = new BinarySearchTree(r);
        }
    } */

    bool search(int value){
        if(value == data)
            return true;
        if(value > data)
            return right && right->search(value) ;
        return left && left->search(value);
    }

    void insert(int target){
        if(target < data){
            if(!left)
                left = new BinarySearchTree(target);
            else
                left->insert(target);
        }
        else if(target > data){
            if(!right)
                right = new BinarySearchTree(target);
            else
                right->insert(target);
        }
    }

    int min_value(){
        BinarySearchTree* cur = this;
        while(cur && cur->left)
            cur = cur->left;
        return cur->data;
    }

    int max_value(){
        BinarySearchTree* cur = this;
        while(cur && cur->right)
            cur = cur->right;
        return cur->data;
    }

    void print_inorder(){
        if(left)
            left->print_inorder();
        cout<<data<<"  ";
        if(right)
            right->print_inorder();
    }

    bool search_iterative(int target){
        BinarySearchTree *node = this;

		while (node) {
			if (target == node->data)
				return true;

			if (target < node->data)
				node = node->left;
			else
				node = node->right;
		}
		return false;
    }

    bool is_bst1(int mn = INT_MIN, int mx = INT_MAX){
        bool status = data > mn && data < mx;

        if(!status)
            return false;

        bool left_bst = !left || left->is_bst1(mn, data);

        if(!left_bst)
            return false;

        bool right_bst = !right || right->is_bst1(data, mx);

		return right_bst;
    }

    void inorder_vector(vector<int> &v){
        if(left)
            left->inorder_vector(v);

        v.push_back(data);

        if(right)
            right->inorder_vector(v);
    }

    bool is_bst2(){
        vector<int> v;
        inorder_vector(v);
        for(int i=0; i<v.size()-1; i++){
            if(v[i] > v[i+1])
                return false;
        }
        return true;
    }

    int kth_smallest(int &k){

        if(left){
            int res = left->kth_smallest(k);
            if(k == 0)
                return res;
        }

        k--;

        if(k == 0)
            return data;

        if(right)
            return right->kth_smallest(k);
        return -1234;
    }

    int LCA(int x,int y){
        // if both on left or right subtree, then lca must be in the subtree
        if(x < data && y < data)
            return left->LCA(x,y);

        if(x > data && y > data)
            return right->LCA(x,y);

        return data;		// otherwise this node is lca: x is in a branch and y in another
    }

    bool find_chain(vector<BinarySearchTree*> &ancestor, int target){
        ancestor.push_back(this);

        if(data == target)
            return true;

        if(data > target)
            return left && left->find_chain(ancestor , target);

        return right && right->find_chain(ancestor , target);
    }

    BinarySearchTree* get_next(vector<BinarySearchTree*> &ancestor){
        if(ancestor.size() == 0)
            return nullptr;
        BinarySearchTree* node = ancestor.back();
        ancestor.pop_back();
        return node;
    }

    pair<bool, int> successor(int target){
        vector<BinarySearchTree*> ancestor;

        if(!find_chain(ancestor, target))
            return make_pair(false, -1);

        BinarySearchTree* child = get_next(ancestor);

        if(child->right)
            return make_pair(true, child->right->min_value());

        BinarySearchTree* parent = get_next(ancestor);

        while(parent && parent->right == child){
            child = parent;
            parent = get_next(ancestor);
        }

        if(parent)
            return make_pair(true, parent->data);

        return make_pair(false, -1);
    }

    void Queries_of_ancestors(deque<int>& q, deque<int>& answer, deque<int>& traversed){
        if(q.empty())
            return;

        if(left && data > q.front()){
            left->Queries_of_ancestors(q, answer, traversed);

            if(q.empty())
                return;
        }

        if(!traversed.empty() && traversed.back() == q.front()){
            answer.push_back(data);
            q.pop_front();

            if(q.empty())
                return;
        }
        traversed.push_back(data);

        if(right && data <= q.front())
            right->Queries_of_ancestors(q, answer, traversed);

    }

    BinarySearchTree* min_node(){
        BinarySearchTree* cur = this;
        while(cur && cur->left)
            cur = cur->left;
        return cur;
    }

    void delete_value(int target){
        if(data == target && !left && !right)
            return;
        delete_node(target, this);
    }

    void special_delete(BinarySearchTree* child){
        data = child->data;
        left = child->left;
        right = child->right;
        delete child;
    }

    BinarySearchTree* delete_node(int target, BinarySearchTree* node){
        if(!node)
            return nullptr;

        if(node->data > target)
            node->left = delete_node(target, node->left);
        else if(node->data < target)
            node->right = delete_node(target, node->right);
        else {
            BinarySearchTree* tmp = node;
            if (!node->left && !node->right) {
				delete node;
				node = nullptr;
			}
            else if(!node->left){
                node->special_delete(node->right);
            }
            else if(!node->right){
                node->special_delete(node->left);
            }
            else {
                BinarySearchTree* mn = node->right->min_node();
                node->data = mn->data;
                node->right = delete_node(node->data, node->right);
            }
        }
        return node;
    }

    //////////////
    BinarySearchTree* max_node(){
        BinarySearchTree* cur = this;
        while(cur && cur->right)
            cur = cur->right;
        return cur;
    }

    void delete_value_p(int target){
        if(data == target && !left && !right)
            return;
        delete_node_p(target, this);
    }

    BinarySearchTree* delete_node_p(int target, BinarySearchTree* node){
        if(!node)
            return nullptr;

        if(node->data > target)
            node->left = delete_node(target, node->left);
        else if(node->data < target)
            node->right = delete_node(target, node->right);
        else {
            BinarySearchTree* tmp = node;
            if (!node->left && !node->right) {
				delete node;
				node = nullptr;
			}
            else if(!node->left){
                node->special_delete(node->right);
            }
            else if(!node->right){
                node->special_delete(node->left);
            }
            else {
                BinarySearchTree* mx = node->left->max_node();
                node->data = mx->data;
                node->left = delete_node(node->data, node->left);
            }
        }
        return node;
    }

    ///////////
    void delete_value_without_recursion(int target){
        if(data == target && !left && !right)
            return;
        delete_node_without_recursion(target, this);
    }

    BinarySearchTree* delete_node_without_recursion(int target, BinarySearchTree* node){
        if(!node)
            return nullptr;

        if(node->data > target)
            node->left = delete_node(target, node->left);
        else if(node->data < target)
            node->right = delete_node(target, node->right);
        else {
            BinarySearchTree* tmp = node;
            if (!node->left && !node->right) {
				delete node;
				node = nullptr;
			}
            else if(!node->left){
                node->special_delete(node->right);
            }
            else if(!node->right){
                node->special_delete(node->left);
            }
            else {
                BinarySearchTree* p = node;
                BinarySearchTree* child = node->right;
                while(child && child->left){
                    p = child;
                    child = child->left;
                }

                node->data = child->data;

                if(p->right == child)
                    p->right = child;
                else
                    p->left = child->right;

                delete child;
            }
        }
        return node;
    }

};



class BinarySearchTreewithparent{
private:
    int data {};
    BinarySearchTreewithparent* left{};
    BinarySearchTreewithparent* right{};
    BinarySearchTreewithparent* parent{};
public:
    BinarySearchTreewithparent(int data) : data(data){}

    void print_inorder(){
        if(left)
            left->print_inorder();

        cout<<data<<"  ";

        if(right)
            right->print_inorder();
    }

    void insert(int val){
        BinarySearchTreewithparent* parent_c = this;
        if(data > val){
            if(left)
                left->insert(val);

            else{
                left = new BinarySearchTreewithparent(val);
                left->parent = parent_c;
            }
        }
        else if(data < val){
            if(right)
                right->insert(val);

            else{
                right = new BinarySearchTreewithparent(val);
                right->parent = parent_c;
            }
        }
    }

    int get_parent(int val){
        if(val == data)
            return parent->data;
        else if(val < data)
            left->get_parent(val);
        else if(val > data)
            right->get_parent(val);
    }

    BinarySearchTreewithparent* find(int target) {
		if (target == data)
			return this;

		if (target < data) {
			if (!left)
				return nullptr;
			return left->find(target);
		}

		if (!right)
			return nullptr;
		return right->find(target);
	}

    int min_value(){
        BinarySearchTreewithparent* cur = this;
        while(cur && cur->left)
            cur = cur->left;
        return cur->data;
    }

    pair<bool, int> successor(int target){
        BinarySearchTreewithparent* child = find(target);
        if(!child)
            return make_pair(false, -1);
        if(child->right)
            return make_pair(true, child->right->min_value());

        BinarySearchTreewithparent* parent_c = child->parent;

        while(parent_c && parent_c->right == child){
            child = parent_c;
            parent_c = child->parent;
        }

        if(parent_c)
            return make_pair(true, parent_c->data);

        return make_pair(false, -1);
    }

};

bool is_degenerate(vector<int> &preorder) {
    if(preorder.size() <= 2)
        return true;

    int mn = 1, mx = 1000;

    for(int i=1; i<preorder.size(); i++){
        if(preorder[i] > mx || preorder[i] < mn)
            return false;

        if(preorder[i] > preorder[i - 1])
            mn = preorder[i - 1] + 1;
        else
            mx = preorder[i - 1] - 1;
    }
    return true;
}
BinarySearchTree* build_balanced_bst_tree(vector<int> &values, int start = 0, int end = -10){
    if(end == -10)
        end = values.size() - 1;

    int mid = (start + end) / 2;

    BinarySearchTree *left = build_balanced_bst_tree(values, start , mid-1);
    BinarySearchTree *right = build_balanced_bst_tree(values, mid+1 , end);
    BinarySearchTree *root = new BinarySearchTree(values[mid],left, right);

    return root;
}

int main(){
    BinarySearchTree tree(50);
	tree.insert(20);
	tree.insert(45);
	tree.insert(70);
	tree.insert(74);
	tree.insert(35);
	tree.insert(15);
	tree.insert(60);
	tree.insert(71);
	tree.insert(72);
	tree.insert(73);

    tree.print_inorder();
    cout<<"\n";
    tree.delete_value_without_recursion(70);
    tree.print_inorder();

    return 0;
}








