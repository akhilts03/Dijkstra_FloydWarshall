//Akhil Thirukonda Sivakumar (2103106)
//Vikrant Suresh Tripathi (2103141)
//Lab 7.2


#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
using namespace std;
#define infinity 1e9 // define infinity as max value of 32 bits integer i.e. 2^31-1
struct neighborList      // structure of a node in the adjacency list
{
    int neighborIndex;         // index of the neighbor
    int weight;                // weight of the edge
    struct neighborList *next; // next pointer
};

class min_heap
{
    int size;
    int volume;
    pair<int, int> *arr;
    
public:
    min_heap(int volume);
    bool isempty() {
        return (size == 0);
    }
    void insert(pair<int, int> c);
    void heapify(int indx);
    pair<int, int> top() {
        return arr[0];
    }
    void pop();
};

void swap(pair<int, int> *t1, pair<int, int> *t2)
{
    pair<int, int> temp = *t2;
    *t2 = *t1;                 // Set the value of t2 to the value of t1
    *t1 = temp;                // Set the value of t1 to the value of "temp" (which was previously t2)
}

// Constructor for the min_heap class
min_heap::min_heap(int v)
{
    volume = v;                  // Set the maximum size of the heap
    size = 0;                    // Set the current size of the heap to zero
    arr = new pair<int, int>[v]; // Allocate an array of pairs of integers on the heap
}

void min_heap::insert(pair<int, int> c)
{
    int indx = size;  // Initialize the index to the last position in the heap
    size++;
    arr[indx] = c;    // Place the new element at the end of the heap
    while (indx != 0)
    {
        int parent = (indx - 1) / 2;             // Calculate the index of the parent node
        if (arr[parent].first > arr[indx].first)
        {
            swap(&arr[parent], &arr[indx]); // Swap the parent and current nodes
            indx = parent;                        // Update the current index to the parent index
        }
        else
            break;
    }
}

void min_heap::heapify(int indx)
{
    if (size == 0)
        return;
    int smallest = indx;
    int left = 2 * indx + 1;  // Calculate the index of the left child
    int right = 2 * indx + 2; // Calculate the index of the right child
    if (left < size && arr[left].first < arr[indx].first)
        smallest = left; // If the left child is smaller than the current index, update the smallest index
    if (right < size && arr[right].first < arr[smallest].first)
        smallest = right; // If the right child is smaller than the smallest index, update the smallest index
    if (smallest != indx)  // If the smallest index is not the current index
    {
        swap(&arr[indx], &arr[smallest]); // swap
        heapify(smallest);
    }
}


void min_heap::pop()
{
   
    if (size <= 0)
        return;
    
    if (size == 1)
    {
        size--;
        return;
    }
   
    arr[0] = arr[size - 1];
    size--;
    heapify(0); // Restore the heap property by heapifying from the root
}

class graph
{
    int vCount;             // vertex count
    int **adjMatrix;        // adjacency matrix
    neighborList **adjList; // adjacency list
    int edgeCount;          // edge count
    int superSource;        // source vertex for Dijkstra's Algorithm
    int *predListDijkstra;  // predecessor list from Dijkstra's Algorithm
    int **predListFW;       // predecessor list from Floyd-Warshall Algorithm
    int *distListDijkstra;  // first details from Dijkstra's Algorithm
    int **distListFW;       // first details from FW Algorithm
public:
    graph(int vertexCount)
    {
        vCount = vertexCount;
        edgeCount = 0;
        allocateMem();
    }

    void allocateMem() // allocates memory for the necessary data structures
    {
        adjMatrix = new int *[vCount];
        adjList = new neighborList *[vCount];
        for (int i = 0; i < vCount; i++)
        {
            adjMatrix[i] = new int[vCount];
            adjList[i] = NULL;
        }

        predListDijkstra = new int[vCount];
        distListDijkstra = new int[vCount];
        predListFW = new int *[vCount];
        distListFW = new int *[vCount];
        for (int i = 0; i < vCount; i++)
        {
            predListFW[i] = new int[vCount];
            distListFW[i] = new int[vCount];
        }
    }
    void readAdjMatrix(FILE *fp) // reads adjacency matrix from the file
    {
        for(int i=0;i<vCount;i++)
            for(int j=0;j<vCount;j++)
                fscanf(fp,"%d",&adjMatrix[i][j]);
    }

    void printAdjMatrix() // prints adjacency matrix
    {// Set the size of the adjacency matrix to n x n
        int n = vCount;
        
        // Iterate through all rows of the adjacency matrix
        for (int i = 0; i < n; i++ ){
            // Iterate through all columns of the adjacency matrix
            for ( int j=0;j<n; j++ ){
                // Print out the value of the element at position (i, j) in the adjacency matrix
                cout << adjMatrix[i][j] << " ";
            }
            // Print a newline character to separate the output for the next row
            cout << endl;
        }
    }
    void loadAdjList() // set up the adjacency list
    {// Iterate through all vertices in the graph
        for(int i=0;i<vCount;i++){
            // Initialize a pointer to a linked list of neighbor nodes
            neighborList * next = NULL;
            
            // Iterate through all vertices again in reverse order
            for(int j= vCount-1;j>=0;j--){
                // If there is an edge between vertices i and j
                if (adjMatrix[i][j] != 0){
                    // Create a new neighbor node
                    neighborList * newlist = new neighborList;
                    // Set the index of the neighbor node
                    newlist->neighborIndex = j+1;
                    // Set the weight of the edge between the vertices
                    newlist->weight = adjMatrix[i][j];
                    // Add the new node to the linked list of neighbors
                    newlist->next = next;
                    next = newlist;
                    // Set the head of the linked list to the new node
                    adjList[i] = newlist;
                }
            }
        }
    }

    void printAdjList() // prints the adjacency list
    {// Iterate through all vertices in the graph
        for (int i =0; i < vCount; i++){
            // Get a pointer to the head of the linked list of neighbors for the current vertex
            neighborList * next = adjList[i];
            
            // Print out the current vertex's index
            std::cout << i+1 ;
            
            // Iterate through the linked list of neighbors for the current vertex
            while (next != NULL){
                // Print out the neighbor's index and the weight of the edge between the current vertex and the neighbor
                std::cout << "->" << next->neighborIndex << " " << next->weight ;
                // Move to the next neighbor in the linked list
                next = next->next;
            }
            
            // Print a newline character to separate the output for the next vertex
            std::cout << "\n";
        }
    }

    int countEdge() // count the number of edges in the graph
    {// Initialize a counter for the number of edges
        
        // Iterate through all vertices in the graph
        for(int i=0;i<vCount;i++){
            // Iterate through all vertices again
            for(int j=0;j<vCount;j++){
                // If there is an edge between vertices i and j
                if (adjMatrix[i][j] != 0){
                    // Increment the edge count
                    edgeCount++;
                }
            }
        }
        return edgeCount; // Return the total number of edges in the graph
    }

    
    //Dijkstra's algorithm: Single pair shortest path
    void runDijkstra(int startVertex)
    {
        superSource = startVertex; 
        for (int i = 0; i < vCount; i++)
        {
            distListDijkstra[i] = infinity; // Set the distance of each vertex to infinity
        }
        distListDijkstra[superSource - 1] = 0;
        predListDijkstra[superSource - 1] = superSource;
        pair<int, int> p1 = make_pair(0, superSource);
        min_heap heaper(vCount + 3);
        heaper.insert(p1);
        // Loop until the minimum heap is empty
        while (!heaper.isempty())
        {
            int node = heaper.top().second;
            neighborList *temp = adjList[node - 1]; // Get the neighborList of the current node
            heaper.pop();
            // Loop through all the neighbors of the current node
            while (temp != NULL)
            {
                int adjnode = temp->neighborIndex;
                int cost = temp->weight;
                if (distListDijkstra[adjnode - 1] > distListDijkstra[node - 1] + cost)
                {
                    distListDijkstra[adjnode - 1] = distListDijkstra[node - 1] + cost; // Update the distance of the neighbor node
                    heaper.insert(make_pair(distListDijkstra[adjnode - 1], adjnode));  // Add the neighbor node to the minimum heap
                    predListDijkstra[adjnode - 1] = node;                              // Update the predecessor of the neighbor node
                }
                temp = temp->next; // Move to the next neighbor
            }
        }
    }

    /*
     Floyd Warshall algorithm:
     1. Initialize distance matrix using loaded adjacency matrix
     2. Initialize predecessor matrix by sending all entries as the row index
     3. Run a loop for a duration equal to the total number of vertices in the graph
     4. In each iteration of the loop, check each entry in the distance matrix.
     5. Replace it with the minimum of the entry or the distance bw i to k and k to j, where k is the iteration.
     6. In doing this, we store the length of the shortest path from i to j with intermediate vertices up to k.
     7. Do this till k reaches the vCount-1.
     8. You now have a distance matrix that represents the shortest distance from i to j in the entry d[i][j].
     */

    void runFW()
    {
        //Initializing distance matrix
        //adjacency matrix is basically the distance matrix for direct paths.
        for (int i = 0; i < vCount; i++)
        {
            for (int j = 0; j < vCount; j++)
            {
                if (adjMatrix[i][j] == 0 && i != j)
                {
                    adjMatrix[i][j] = 100000;   //If the entry is 0 and it's not a diagonal entry => there exists no path from i to j. Set it to infinity.
                }
                distListFW[i][j] = adjMatrix[i][j]; 
            }
        }

        //populating the predecessor matrix.
        for (int i = 0; i < vCount; i++)
        {
            for (int j = 0; j < vCount; j++)
            {
                if (distListFW[i][j] != 100000)
                {
                    predListFW[i][j] = i; //If there is direct path, for now just set it to the start vertex, i.e, i.
                }
            }
        }

        for (int i = 0; i < vCount; i++)
        {
            predListFW[i][i] = i;    //set all the diagonal entries to the index itself as they form loops.
        }


        //updating the distance and the predecessor matrices.
        for (int t = 1; t < vCount; t++)
        {
            for (int i = 0; i < vCount; i++)
            {
                for (int j = 0; j < vCount; j++)
                {
                    if (distListFW[i][t] + distListFW[t][j] < distListFW[i][j])  //update only if better path is found.
                    {
                        predListFW[i][j] = predListFW[t][j];
                        distListFW[i][j] = distListFW[i][t] + distListFW[t][j];
                    }
                }
            }
        }

        //code used to print the distance matrix and the predecessor matrix for debugging.
        // for (int m = 0; m < vCount; m++)
        // {
        //     for (int n = 0; n < vCount; n++)
        //     {
        //         cout << distListFW[m][n] << " ";
        //     }
        //     cout << endl;
        // }
        // cout << endl;
        // for (int m = 0; m < vCount; m++)
        // {
        //     for (int n = 0; n < vCount; n++)
        //     {
        //         cout << predListFW[m][n] << " ";
        //     }

        //     cout << endl;
        // }
        // cout << endl;
    }

    //printing Dijkstra path:
    void printDijkstraPathTo(int targetVertex)
    {
        vector<int> ans(vCount);
        cout << "The shortest path from " << superSource << " to " << targetVertex << " costs: " << distListDijkstra[targetVertex - 1] << endl;
        int j = 0;
        for (int i = targetVertex; i != superSource; i = predListDijkstra[i - 1])
        {
            ans[j++] = i;
        }
        ans[j] = superSource;
        cout << "The path is ";
        for (int i = j; i >= 0; i--)
        {
            cout << ans[i];
            if (i != 0)
                cout << "->";
        }
        cout << endl;
    }

    /*Printing Floyd Warshall path:
    1. We take two inputs: the source vertex and the target vertex
    2. We first look up the predecessor table for the (start,target) entry
    3. It's entry shows the immediate predecessor of the target vertex in the shortest path from start to target, say x.
    4. we then look up the entry (start,x) which now shows the predecessor of x in the same path.
    5. we keep repeating this pattern until the predecessor is the start vertex itself.
    6. we store these predecessors and finally print them as a path from start to the target vertex.
    7. The length of the path is taken from the distance matrix.
    */

	void printFWPathBetween(int sourceVertex, int targetVertex)/*sourceVertex->......->targetVertex     lengthOfPath*/
	{

        int start = sourceVertex-1;  //vertices start from 1 but indices start from 0 so we reduce 1 from the input.
        int end = targetVertex-1;
        int length = distListFW[start][end];
        while (predListFW[start][end]!=start)
        {
            cout<<end+1<<"<-";      //print out the index + 1 = vertex.
            end = predListFW[start][end];
        }

        cout<<end+1<<"<-"<<start+1<<"  "<<"Length of path:"<<length<<endl;   
	}


    //menu to test Floyd Warshall algorithm.
    void testFW(){
        while (true)
        {
            int x = 0;
            cout<<"\nPress 1 to run the Floyd Warshall algorithm and populate the tables."<<endl;
            cout<<"Press 2 to find the shortest path between two vertices"<<endl;
            cout<<"Press 3 to quit"<<endl;
            cin>>x;
            if (x == 1)
            {
                runFW();
            }
            if (x == 2)
            {
                int a = 0;
                int b = 0;
                cout<<"Enter start vertex: ";
                cin>>a;
                cout<<"\nEnter end vertex: ";
                cin>>b;
                cout<<endl;
                printFWPathBetween(a,b);
            }
            if (x == 3)
            {
                break;
            }
        }       
    }

    //menu to test Dijkstra's algorithm
    void testDijkstra(){
        while (true)
        {
            int x = 0;
            cout<<"\nPress 1 to run the Dijkstra's algorithm and populate the tables."<<endl;
            cout<<"Press 2 to find the shortest path to target vertex"<<endl;
            cout<<"Press 3 to quit"<<endl;
            cin>>x;
            if (x == 1)
            {
                int b = 0;
                cout<<"Enter supersource vertex:";
                cin>>b;
                runDijkstra(b);
                cout<<"populated"<<endl;
            }
            else if (x == 2)
            {
                int a = 0;
                cout<<"Enter target vertex:";
                cin>>a;
                cout<<endl;
                printDijkstraPathTo(a);
            }
            else if (x == 3)
            {
                break;
            }
        }
    }

}*myGraph;


int main(int argc, char **argv)
{
    int vertexCount;
    int source;
    int target;
    FILE *fp = fopen("graph.txt", "r");
    fscanf(fp, "%d", &vertexCount);
    myGraph = new graph(vertexCount); // initialize vertex count from the file

    myGraph->readAdjMatrix(fp);
    fclose(fp);

    printf("\n There are %d edges in the graph\n", myGraph->countEdge());

    myGraph->loadAdjList();

    //myGraph->printAdjMatrix();
    //myGraph->printAdjList();

    myGraph->testDijkstra();
    myGraph->testFW();
}
