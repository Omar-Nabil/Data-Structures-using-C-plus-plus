#include <iostream>
#include <cassert>
#include <climits>
#include <stack>	// for debug
#include <vector>		// for debug
#include <algorithm>
#include<queue>
#include<deque>
#include<string>

using namespace std;

class BinaryTree{
private:
    int data;
    BinaryTree* left = nullptr;
    BinaryTree* right = nullptr;

public:
    BinaryTree(int data): data(data){

    }


    void print_postorder() {
		if (left)
			left->print_postorder();

		if (right)
			right->print_postorder();
		cout << data;
	}

    BinaryTree(string postfix) {
		stack<BinaryTree*> tree;

		for (int i = 0; i < (int) postfix.size(); ++i) {
			BinaryTree* cur = new BinaryTree(postfix[i]);

			if (!isdigit(postfix[i])) {
				cur->right = tree.top();
				tree.pop();
				cur->left = tree.top();
				tree.pop();
			}
			tree.push(cur);
		}
		BinaryTree* root = tree.top();
		this->data = root->data;
		this->left = root->left;
		this->right = root->right;
	}



    bool is_just_num() {
		// If it has no children, then it is just a number
		return !left && !right;
	}

    void  print_inorder_expression(){
        if(left){
            if(!left->is_just_num())
                cout<<"(";
            left->print_inorder_expression();
            if(!left->is_just_num())
                cout<<")";
        }
        cout<<data<<" ";
        if(right){
            if(!right->is_just_num())
                cout<<"(";
            right->print_inorder_expression();
            if(!right->is_just_num())
                cout<<")";
        }
    }


    void add(vector<int> values, vector<char> direction){
        assert(values.size() == direction.size());
        BinaryTree* current = this;
        for(int i=0; i<values.size(); i++){
            if(direction[i] == 'L'){
                if(!current->left)
                    current->left = new BinaryTree(values[i]);
                else
                    assert(current->left->data == values[i]);
                current = current->left;
            }
            else{
                if(!current->right)
                    current->right = new BinaryTree(values[i]);
                else
                    assert(current->right->data == values[i]);
                current = current->right;
            }
        }
    }

    int tree_max(){
        int val = data;
        if(left)
            val = max(val, left->tree_max());
        if(right)
            val = max(val, right->tree_max());
        return val;
    }

    int tree_height(){
        int res = 0;
        if(left)
            res = 1 + left->tree_height();
        if(right)
            res = max(res, 1 + right->tree_height());
        return res;
    }

    int total_nodes(){
        int cnt = 1;
        if(left)
            cnt +=  left->total_nodes();
        if(right)
            cnt += right->total_nodes();
        return cnt;
    }

    int total_leaf_nodes(){
        int res = 0;
        if(!left && !right)
            return 1;
        if(left)
            res += left->total_leaf_nodes();
        if(right)
            res += right->total_leaf_nodes();
        return res;
    }

    bool is_exists(int value){
        bool is_exist = false;
        if(data == value)
            is_exist = true;
        if(left && !is_exist)
            is_exist = left->is_exists(value);
        if(right && !is_exist)
            is_exist = right->is_exists(value);

        return is_exist;
    }

    bool is_perfect(){
        bool res = (right && left) || (!right && !left);
        if(!right&&left || !left&&right)
            res = false;
        if(left && res)
            res = left->is_perfect();
        if(right && res)
            res = right->is_perfect();
        return res;
    }
    void print_inorder_iterative(){
        stack<pair<BinaryTree*,bool>> nodes;
        nodes.push(make_pair(this, false));

        while(!nodes.empty()){
            BinaryTree* current = nodes.top().first;
            bool is_done = nodes.top().second;

            nodes.pop();

            if(is_done)
                cout<<current->data<<" ";
            else{
                if(current->right)
                    nodes.push(make_pair(current->right, false));

                nodes.push(make_pair(current, true));

                if(current->left)
                    nodes.push(make_pair(current->left, false));
            }
        }

    }

    void traverse_left_boundry(){
        cout<<data<<"  ";
        if(left)
            left->traverse_left_boundry();
        else if(right)
            right->traverse_left_boundry();
    }

    pair<int, int> tree_diameter(){         // return diam & height
        pair<int, int> res = make_pair(0, 0);

        if(!left && !right)
            return res;

        pair<int, int> left_diameter = make_pair(0, 0);
        pair<int, int> right_diameter = make_pair(0, 0);

        if(left)
            left_diameter = left->tree_diameter(), res.first += 1 + left_diameter.second;
        if(right)
            right_diameter = right->tree_diameter(), res.first += 1 + right_diameter.second;

        res.first = max(res.first ,max(left_diameter.first, right_diameter.first));

        res.second = 1 + max(left_diameter.second, right_diameter.second);

        return res;
    }

    void level_order_traversal1(){
        queue<BinaryTree*> q;
        BinaryTree* cur = this;
        q.push(cur);
        while(!q.empty()){
            cout<<q.front()->data<<" ";
            cur = q.front();
            q.pop();
            if(cur->left)
                q.push(cur->left);
            if(cur->right)
                q.push(cur->right);
        }
        cout<<"\n";
    }

    void level_order_traversal2(){
        queue<BinaryTree*> q;
        BinaryTree* cur = this;
        q.push(cur);
        int sz;
        int level = 0;
        while(!q.empty()){
            cout<<"nodes in level "<<level<<"\n";
            sz  = q.size();
            while(sz--){
                cout<<q.front()->data<<" ";
                cur = q.front();
                q.pop();
                if(cur->left)
                    q.push(cur->left);
                if(cur->right)
                    q.push(cur->right);
            }
            level++;
            cout<<"\n";
        }
        cout<<"\n";
    }

    void print_nodes_level(int level, bool left_to_right = true){
        if(level == 0)
            cout<<data<<" ";
        else if(level){
            if(left_to_right){
                if(left)
                    left->print_nodes_level(level-1,left_to_right);
                if(right)
                    right->print_nodes_level(level-1,left_to_right);
            }
            else{
                if(right)
                    right->print_nodes_level(level-1,left_to_right);
                if(left)
                    left->print_nodes_level(level-1,left_to_right);
            }
        }
    }

    void level_order_traversal_recursive(){
        int h = tree_height() ;

        for(int i=0; i<=h; i++)
            print_nodes_level(i);
    }

    void level_order_traversal_spiral(){
        int h = tree_height() ;
        bool left_to_right = true;
        for(int i=0; i<=h; i++){
            print_nodes_level(i, left_to_right);
            left_to_right = !left_to_right;
        }
    }


    bool is_complete(){
        queue<BinaryTree*> q;
        q.push(this);
        bool no_more_allowed = false;
        while(!q.empty()){
            int sz = q.size();
            while(sz--){
                BinaryTree* cur = q.front();
                q.pop();
                if(cur->left){                   // we check left before right, so it is impossible to return true if node has right and
                                                    // didn't have left
                    if (no_more_allowed)
						return false;
                    q.push(cur->left);
                }
                else
                    no_more_allowed = true;

                if(cur->right){
                    if (no_more_allowed)
						return false;
                    q.push(cur->right);
                }
                else
                    no_more_allowed = true;
            }

        }
        return true;
    }

     BinaryTree(deque<int> &preorder, deque<int> &inorder){
        int item = preorder.front();
        deque<int> left {};
        deque<int> right {};
        BinaryTree* cur = new BinaryTree(item);
        preorder.pop_front();
        int i;
        for( i=0; i<inorder.size(); i++){
            if(inorder[i] == item)
                break;
            left.push_back(inorder[i]);
        }
        for(i=i+1; i<inorder.size(); i++){
            right.push_back(inorder[i]);
        }
        if(!left.empty())
            cur->left = new BinaryTree(preorder, left);
        if(!right.empty())
            cur->right = new BinaryTree(preorder, right);

        this->data = cur->data;
		this->left = cur->left;
		this->right = cur->right;
     }

     BinaryTree(queue<pair<int, int>> &preorder_queue){
        pair<int,int> p = preorder_queue.front();
        preorder_queue.pop();

        data = p.first;

        if(!p.second){
            left = new BinaryTree(preorder_queue);
            right = new BinaryTree(preorder_queue);
        }
     }

     void print_preorder_complete(){
        cout<<data<<" ";
        if(left)
            left->print_preorder_complete();
        else
            cout<<"-1 ";
        if(right)
            right->print_preorder_complete();
        else
            cout<<"-1 ";
     }

     string parenthesize(bool left_first = true){
        string res = "(" + to_string(data);

        if(left_first){
            if(left)
                res += left->parenthesize(left_first);
            else
                res += "()";
            if(right)
                res += right->parenthesize(left_first);
            else
                res += "()";

        }
        else{
            if(right)
                res += right->parenthesize(left_first);
            else
                res += "()";

            if(left)
                res += left->parenthesize(left_first);
            else
                res += "()";
        }
        res += ")";
        return res;
     }

     string parenthesize_canonical(){
        string repr = "(" + to_string(data);

        vector<string> v;

        if(left)
            v.push_back(left->parenthesize_canonical());
        else
            v.push_back("()");

        if(right)
            v.push_back(right->parenthesize_canonical());
        else
            v.push_back("()");

        sort(v.begin(), v.end());
        for(int i=0; i<v.size(); i++){
            repr += v[i];
        }
        repr += ")";

        return repr;
     }

    bool Symmetric_Tree(){
        if(!left && !right)
            return true;
        if(!left&&right || left&&!right)
            return false;
        if(left->parenthesize() ==  right->parenthesize(false))
            return true;
        else
            return false;
    }

    bool is_mirror(BinaryTree* first, BinaryTree* second){
        if(!first && !second)
            return true;
        if(!first&&second || first&&!second || first->data != second->data)
            return false;
        return is_mirror(first->left, second->right) && is_mirror(first->right, second->left);
    }

    bool Symmetric_Tree2(){
        return is_mirror(left, right);
    }

    bool is_flip_equiv2(BinaryTree* other){
        bool res = true;
        if(data != other->data)
            res = false;
        if(left && other->left && res){
            if(left-> data != other->left->data){
                swap(left, right);
                if(left-> data != other->data)
                    res = false;
                left->is_flip_equiv(other->left);
            }
        }
        else
            res = false;

        if(right && other->right && res){
            if(right-> data != other->right->data){
                swap(left, right);
                if(right-> data != other->data)
                    res = false;
                right->is_flip_equiv(other->right);
            }
        }
        else
            res = false;

        return res;

    }

    bool is_flip_equiv(BinaryTree* other){
        return parenthesize_canonical() == other->parenthesize_canonical();
    }

    string parenthesize(vector<string> &v){
        string str = "(" + to_string(data);
        if(left)
            str += left->parenthesize(v);
        else
            str += "()";
        if(right)
            str += right->parenthesize(v);
        else
            str += "()";
        str += ")";
        if(left || right)
            v.push_back(str);

        return str;
    }

    void all_duplicate_subtrees(){
        vector<string> v;
        parenthesize(v);
        sort(v.begin(), v.end());
        v.push_back("#");

        for(int i=0; i<v.size();){
            int j = i + 1;
            while(j<v.size() && v[i] == v[j])
                j++;
            if(j > i+1)
                cout<<v[i]<<"\n";
            i = j;
        }
    }

};



int main() {
    BinaryTree tree(1);

	tree.add( { 2, 3 }, { 'L', 'L'});
	tree.add( { 4, 5, 6, 8, 9 }, { 'R', 'R', 'R', 'R', 'R' });
	tree.add( { 4, 2, 3 }, { 'R', 'L', 'L'});
	tree.add( { 4, 5, 6, 7 }, { 'R', 'R', 'L', 'L'});
	tree.add( { 4, 5, 6, 8, 9 }, { 'R', 'R', 'L', 'R', 'R'});
	tree.add( { 4, 5, 6, 7 }, { 'R', 'R', 'R', 'L'});

    tree.all_duplicate_subtrees();
	return 0;
}

