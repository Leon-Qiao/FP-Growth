#include <iostream>
#include <vector>
#include <set>
#include <cstring>
using namespace std;

const float Minsup = 0.25;
struct itemSet{
    char name;
    int count;
    vector<string> patternKey;
    vector<int> patternCount;
};
class FP_Tree{
public:
    struct Node{
        char name;
        int count;
        Node *parent;
        vector<Node*> children;
    };
    Node *_root;
    
    FP_Tree(Node *root) : _root(root) {}
    
    void findFromNode(Node *node,vector<char> aim,int now){
        if(aim.size()==now)
            return;
        if(node){
            for(int i=0;i<node->children.size();i++)
                if(node->children[i]->name == aim[now]){
                    node->children[i]->count++;
                    findFromNode(node->children[i], aim, ++now);
                    return;
                }
            Node *temp = new Node();
            temp->name = aim[now];
            temp->count = 1;
            temp->parent = node;
            node->children.emplace_back(temp);
            findFromNode(node->children[node->children.size()-1], aim, ++now);
        }
    }
    void showTree(){
        vector<vector<Node*>> nodeList;
        vector<Node*> temp;
        temp.push_back(_root);
        nodeList.push_back(temp);
        temp.clear();
        for(int i=0;;i++){
            int flag = 0;
            for(int j=0;j<nodeList[i].size();j++){
                cout<<nodeList[i][j]->name<<nodeList[i][j]->count<<" ";
                for(int k=0;k<nodeList[i][j]->children.size();k++){
                    temp.push_back(nodeList[i][j]->children[k]);
                    flag = 1;
                }
            }
            nodeList.push_back(temp);
            temp.clear();
            cout<<endl;
            if(flag == 0)
                break;
        }
    }
    void PreOrderDFS(Node *seed,itemSet &aim){
        if(seed){
            if(seed->name==aim.name){
                Node *temp = seed;
                string key="";
                while(temp->parent != _root){
                    temp = temp->parent;
                    key = temp->name + key;
                }
                aim.patternKey.push_back(key);
                aim.patternCount.push_back(seed->count);
            }
            for(int i=0;i<seed->children.size();i++)
                PreOrderDFS(seed->children[i], aim);
        }
    }
};

vector<vector<char>> dataSet = {{'a','b','c','e'},
                    {'c','d','e','h'},
                    {'a','d','f','h'},
                    {'a','h'},
                    {'b','c','e'},
                    {'a','b','f','h'},
                    {'b','d','f','h'},
                    {'c','f','g'}};
int MinsupNum = Minsup * dataSet.size();
vector<itemSet> L1;
bool GreaterSort (itemSet a,itemSet b) { return (a.count>b.count); }
bool LessSort (itemSet a,itemSet b) { return (a.count<b.count); }
int& scanD(const char c){
    for(int i=0;i<L1.size();i++)
        if(c==L1[i].name)
            return L1[i].count;
    itemSet item = {c,0};
    L1.push_back(item);
    return L1[L1.size()-1].count;
}
/*
void toPattern(itemSet item){
    set<char> key[10];
    for(int i=0;i<item.patternKey.size();i++){
        for(int j=0;j<sizeof(item.patternKey[i]);j++){
            key[i].
        }
    }
}
*/
int main(){
    for(int i=0;i<dataSet.size();i++)
        for(int j=0;j<dataSet[i].size();j++)
            scanD(dataSet[i][j])++;

    sort(L1.begin(),L1.end(),GreaterSort);
    for(int i=L1.size()-1;i>=0;i--)
        if(L1[i].count<MinsupNum)
            L1.pop_back();
        else
            break;
    
    cout<<"Show L1 Set:"<<endl;
    for(int i=0;i<L1.size();i++)
        cout<<L1[i].name<<L1[i].count<<" ";
    cout<<endl;
    
    FP_Tree::Node root;
    root.name = '-';
    root.count = 0;
    FP_Tree tree(&root);
    for(int i=0;i<dataSet.size();i++)
        tree.findFromNode(tree._root, dataSet[i],0);
    
    cout<<endl<<"Show Tree:"<<endl;
    tree.showTree();
    cout<<endl;
    
    cout<<"Show patternKey:"<<endl;
    for(int i=L1.size()-1;i>=0;i--){
        tree.PreOrderDFS(tree._root,L1[i]);
        cout<<L1[i].name<<":";
        for(int j=0;j<L1[i].patternKey.size();j++)
            cout<<L1[i].patternKey[j]<<L1[i].patternCount[j]<<" ";
        cout<<endl;
    }
    return 0;
}
