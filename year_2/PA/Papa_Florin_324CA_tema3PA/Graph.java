//Papa Florin, 324CA

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.util.Stack;
import java.util.Vector;

public class Graph {

    static class Edge {
        
        public int x, y, c;

        public Edge(int x, int y, int c) {
            
            this.x = x;
            this.y = y;
            this.c = c;
        }
   }

   static enum Type {
       
        DIRECTED,

        UNDIRECTED
   }

   public final static int UNDEFINED = 0;

   public final static int INFINITY = Integer.MAX_VALUE;

   public boolean[][] adjMatrix;

   public int[][] capacityMatrix;

   public int[][] flow;

   private Vector<Vector<Integer>> neigh;
   private Vector<Edge> edges;
   private Vector<Integer> nodes;

   private int[] parents;
   private int size;

   private String filename = null;
   private Graph.Type type = null;

   public Graph.Type getType() {
       
        return type;
   }

   public Graph(String filename, Graph.Type type) throws FileNotFoundException {

        this.filename = filename;
        this.type = type;

        load(filename, type);
   }

   public Graph(int size, Graph.Type type) {
       
        newEmptyGraph(size, type);
   }

   public void reloadFromDisk() throws FileNotFoundException {
        if (filename == null) {
            
            throw new FileNotFoundException();
        } else {
            
            load(filename, type);
        }
   }

   private void newEmptyGraph(int size, Graph.Type type) {
       
        this.size = size;
        this.type = type;

        allocObjects();
        initAdjMatrix();
        initCapacityMatrix();
        clearParents();
        clearFlow();
   }

   private void load(String filename, Graph.Type type) throws FileNotFoundException {
       
        Scanner inputScanner = new Scanner(new File(filename));

        int size = inputScanner.nextInt();
        int edgeCount = inputScanner.nextInt();

        newEmptyGraph(size, type);

        for (int i = 0; i < edgeCount; i++) {
            
            int x = inputScanner.nextInt();
            int y = inputScanner.nextInt();
            int c = inputScanner.nextInt();

            addEdge(x, y, c);
        }

        inputScanner.close();
   }

   public void addEdge(int x, int y, int c) {
       
        _addEdge(x, y, c);
        
        if (type == Graph.Type.UNDIRECTED)
            _addEdge(y, x, c);
   }

   private void _addEdge(int x, int y, int c) {
       
        adjMatrix[x][y] = true;
        capacityMatrix[x][y] = c;
        neigh.get(x).add(y);
        edges.add(new Edge(x, y, c));
   }

   private void initCapacityMatrix() {
       
        for (int i = 0; i < size; i++) {
            
            for (int j = 0; j < size; j++) {
                
                capacityMatrix[i][j] = UNDEFINED;
            }
        }
   }

   private void initAdjMatrix() {
       
        for (int i = 0; i < size; i++) {
            
            for (int j = 0; j < size; j++) {
                
                capacityMatrix[i][j] = UNDEFINED;
            }
        }
   }

   public void clearParents() {
       
        for (int i = 0; i < size; i++) {
            
            parents[i] = UNDEFINED;
        }
   }

   public void setParent(int node, int parent) {
       
        parents[node] = parent;
   }

   public int getParent(int node) {
       
        return parents[node];
   }

   public boolean isSaturated(int u, int v) {
       
        return capacityMatrix[u][v] == 0 && isEdge(u, v);
   }

   public void printPathBetween(int first, int last) {
       
        System.out.print("" + first + " ");
        Stack<Integer> path = new Stack<Integer>();
        int node = last;
        do {
            path.push(node);
            node = getParent(node);
        } while (node != first);
        while (!path.empty()) {
            node = path.pop();
            System.out.print("" + node + " ");
        }
        System.out.println();
   }

   private void allocObjects() {

        /* Adj, Cost and Flow matrixes */
        adjMatrix = new boolean[size][size];
        capacityMatrix = new int[size][size];
        flow = new int[size][size];

        /* Neighbours */
        neigh = new Vector<Vector<Integer>>();
        for (int i = 0; i < size; i++) {
            neigh.add(new Vector<Integer>());
        }

        /* Parents */
        parents = new int[size];

        /* Edges */
        edges = new Vector<Graph.Edge>();

        /* Nodes */
        nodes = new Vector<Integer>();
        for (int i = 0; i < size; i++) {
            nodes.add(i);
        }
   }

   public void printAdjMatrix() {
       
        System.out.println("Matrice de adiacenta:");

        for (int i = 0; i < size; i++) {

           for (int j = 0; j < size; j++) {

               System.out.print(adjMatrix[i][j] ? "1 " : "0 ");
           }
           System.out.println();
        }
   }

   public void printCapacityMatrix() {
       
        System.out.println("Matricea costurilor:");
        for (int i = 0; i < size; i++) {

           for (int j = 0; j < size; j++) {

                System.out.print("" + (capacityMatrix[i][j] == UNDEFINED ? "U" : capacityMatrix[i][j]) + " ");
           }
           System.out.println();
        }
   }

   public void printFlowMatrix() {
       
        System.out.println("Matricea de flux:");
        for (int i = 0; i < size; i++) {
            
            for (int j = 0; j < size; j++) {

                System.out.print("" + (flow[i][j] == UNDEFINED ? "U" : flow[i][j]) + " ");
            }
            System.out.println();
        }
   }

   public void printParents() {
       
        System.out.println("Parinti:");
        for (int i = 0; i < size; i++) {
            
           if (parents[i] == UNDEFINED) {
               
               System.out.println("" + i + " : NEDEFINIT");
           } else {
               
               System.out.println("" + i + " : " + parents[i]);
           }
        }
   }

   public void printNeighbours() {
       
        System.out.println("Liste vecini:");
        for (int i = 0; i < size; i++) {

            System.out.print("" + i + " : ");
            for (Integer v : neigh.get(i)) {

                System.out.print("" + v + " ");
            }
            System.out.println();
        }
   }

   public void clearFlow() {
       
        for (int i = 0; i < size; i++) {

           for (int j = 0; j < size; j++) {

               flow[i][j] = 0;
           }
        }
   }

   public void print() {
       
        printNeighbours();
        printAdjMatrix();
        printCapacityMatrix();
        printFlowMatrix();
        printParents();
        printEdges();
        printNodes();
   }

   public void printNodes() {
       
        System.out.print("Noduri: ");
        for (int node : getNodes()) {

           System.out.print("" + node + " ");
        }
        System.out.println();
   }

   public void printEdges() {
       
        System.out.println("Arce: ");
        for (Edge e : getEdges()) {

            System.out.print("(" + e.x + ", " + e.y + ", " + e.c + ") ");
        }
        System.out.println();
   }

   public int getSize() {
       
        return size;
   }

   public boolean isEdge(int x, int y) {
     
        return adjMatrix[x][y];
   }

   public Vector<Integer> getNeighbours(int node) {
       
        return neigh.get(node);
   }

   public Vector<Edge> getEdges() {
       
        return edges;
   }

   public Vector<Integer> getNodes() {
       
        return nodes;
   }
}
