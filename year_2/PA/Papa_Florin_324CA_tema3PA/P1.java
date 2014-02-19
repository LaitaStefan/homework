//Papa Florin, 324CA

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.LinkedList;

public class P1 {
    
    final static int NONE = -1;
    
    public static ArrayList<Integer> bfs(Graph g, int u, int v) {

        ArrayList<Integer> parent = new ArrayList<Integer>(g.getSize());

        for (int i = 0; i < g.getSize(); ++i) {
            parent.add(NONE);
        }

        LinkedList<Integer> q = new LinkedList<Integer>();
        q.add(u);

        while (parent.get(v) == NONE && q.size() > 0) {
            int node = q.get(0);
            q.remove(0);

            for (int i = 0; i < g.getSize(); ++i) {
                if (g.capacityMatrix[node][i] > 0 && parent.get(i) == NONE) {
                    parent.set(i, node);
                    q.add(i);
                }
            }
        }

        /* Daca nu s-a atins destinatia, atunci nu mai exista drumuri de crestere. */
        if (parent.get(v) == NONE) {
            return new ArrayList<Integer>(0);
        }

        /* Reconstituim drumul de la destinatie spre sursa. */
        ArrayList<Integer> returnValue = new ArrayList<Integer>();
        for (int node = v; ; node = parent.get(node)) {
            returnValue.add(0, node);

            if (node == u) {
                break;
            }
        }

        return returnValue;
    }

    public static int saturate_path(Graph g, ArrayList<Integer> path) {

        if (path.size() < 2) {
            return 0;
        }

        /* Determinam fluxul maxim prin drum. */
        int flow = g.capacityMatrix[path.get(0)][path.get(1)];
        for (int i = 0; i < path.size() - 1; ++i) {
            int u = path.get(i), v = path.get(i + 1);

            int minFlow = g.capacityMatrix[u][v];

            if(minFlow < flow) {

                flow = minFlow;

            }
        }
        
        /* Si il saturam in graf. */
        for (int i = 0; i < path.size() - 1; ++i) {
            int u = path.get(i), v = path.get(i + 1);

            g.capacityMatrix[u][v] -= flow;
            g.capacityMatrix[v][u] += flow;      
        }

        /* Raportam fluxul cu care am saturat graful. */
        return flow;
    }

    public static int maxFlow(Graph g, int u, int v) {
        int maxFlow = 0;

        while (true) {
            /* Determina drum de crestere. */
            ArrayList<Integer> saturation_path = bfs(g, u, v);

            int flow = saturate_path(g, saturation_path);

            if(flow <= 0) break;

            maxFlow += flow;
        }

        return maxFlow;
    }
  
    public static void main(String arg[]) throws IOException{
        
        InputParser p = new InputParser("fotbal.in");
        BufferedWriter bw = new BufferedWriter(new FileWriter("fotbal.out"));
        
        for(int i = 0; i < p.nr_queries; i++) {
            
            Graph g = p.getGraph(p.queries[i]);
            int count = 0, edges = p.getLeftNodes();
            
            if(p.negative) {
                
                bw.write("FALSE");
                bw.newLine();
                
                p.negative = false;
                
                continue;
            }
            
            maxFlow(g, 0, p.getLastNode());
            
            for(int j = 1; j <= edges; j++) {
                
                if(!g.isSaturated(0, j)) {

                    count++;
                }
            }
            
            if(count == 0) {
                
                bw.write("TRUE");
                bw.newLine();
            }
            else {
                
                bw.write("FALSE");
                bw.newLine();
            }
        }
        
        bw.close();
    }
}
