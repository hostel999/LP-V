#include<iostream>
#include<queue>
#include<stack>
#include<vector>
#include<omp.h>
using namespace std;

// ── BFS on Binary Tree ──
struct Node {
    int data;
    Node *left, *right;
    Node(int v) { data=v; left=right=NULL; }
};

Node* insert(Node* root, int val) {
    if(!root) return new Node(val);
    queue<Node*> q;
    q.push(root);
    while(!q.empty()) {
        Node* t = q.front(); q.pop();
        if(!t->left)  { t->left  = new Node(val); return root; }
        else q.push(t->left);
        if(!t->right) { t->right = new Node(val); return root; }
        else q.push(t->right);
    }
    return root;
}

void bfs(Node* root) {
    if(!root) return;
    queue<Node*> q;
    q.push(root);
    while(!q.empty()) {
        int sz = q.size();
        #pragma omp parallel for
        for(int i=0; i<sz; i++) {
            Node* curr;
            #pragma omp critical
            { curr = q.front(); q.pop(); cout << curr->data << " "; }
            #pragma omp critical
            { if(curr->left)  q.push(curr->left);
              if(curr->right) q.push(curr->right); }
        }
    }
}

// ── DFS on Undirected Graph ──
const int MAX = 1000;
vector<int> graph[MAX];
bool visited[MAX] = {false};

void dfs(int start) {
    stack<int> s;
    s.push(start);
    while(!s.empty()) {
        int curr;
        #pragma omp critical
        { curr = s.top(); s.pop(); }

        if(!visited[curr]) {
            #pragma omp critical
            { visited[curr]=true; cout << curr << " "; }

            #pragma omp parallel for
            for(int i=0; i<(int)graph[curr].size(); i++) {
                int next = graph[curr][i];
                if(!visited[next])
                    #pragma omp critical
                    { s.push(next); }
            }
        }
    }
}

// ── Main ──
int main() {
    // BFS
    int n; cout << "Enter number of tree nodes: "; cin >> n;
    Node* root = NULL;
    cout << "Enter values: ";
    for(int i=0,x; i<n; i++) { cin>>x; root=insert(root,x); }
    cout << "BFS: "; bfs(root); cout << "\n";

    // DFS
    int nodes, edges, start;
    cout << "Enter nodes, edges, start: "; cin >> nodes >> edges >> start;
    cout << "Enter edges (u v):\n";
    for(int i=0,u,v; i<edges; i++) {
        cin>>u>>v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }
    cout << "DFS: "; dfs(start); cout << "\n";
    return 0;
}

/*
── HOW TO RUN ON UBUNTU ──

1. Install g++ (if needed):
   sudo apt update && sudo apt install g++

2. Compile:
   g++ -fopenmp hpc3.cpp -o hpc3

3. Run:
   ./hpc3

── SAMPLE INPUT ──

Enter number of tree nodes: 5
Enter values: 1 2 3 4 5
Enter nodes, edges, start: 5 4 0
Enter edges (u v):
0 1
0 2
1 3
1 4

── SAMPLE OUTPUT ──

BFS: 1 2 3 4 5
DFS: 0 2 1 4 3

Note: DFS order may vary due to parallel execution.
*/
