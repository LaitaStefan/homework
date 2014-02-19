//Papa Florin, 324CA

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.Vector;

public class InputParser {
    
    int INF = 50000;
    
    boolean negative;
    int nr_teams;
    int played_so_far;
    int stages;
    int nr_queries;
    int[] points;
    int[] remaining_games; //jocuri in total
    int[] queries;
    int[][] games_to_play; //jocuri intre fiecare echipa
    
    public InputParser(String file) throws FileNotFoundException, IOException {
        
        String line;
        String[] tok;
        BufferedReader r = new BufferedReader(new FileReader(file));
        
        line = r.readLine();
        
        getParams(line, r);    //initializeaza parametrii de mai sus
        
        r.close();
               
    }
    
    public void getParams(String s, BufferedReader br) throws IOException{
        
        String[] tok = s.split(" ");
        
        nr_teams = Integer.parseInt(tok[0]);
        played_so_far = Integer.parseInt(tok[1]);
        stages = Integer.parseInt(tok[2]);
        nr_queries = Integer.parseInt(tok[3]);
        
        init(); //initializeaza vectorii
        
        //citesc jocurile jucate pana acum
        for(int i = 0; i < played_so_far; i++) {
            
            String str = br.readLine();
            String[] tokens = str.split(" ");
            
            int t1 = Integer.parseInt(tokens[0]);
            int t2 = Integer.parseInt(tokens[1]);
            
            remaining_games[t1]--;
            remaining_games[t2]--;
            games_to_play[t1][t2]--;
            games_to_play[t2][t1]--;
            
            if(tokens[2].equals("WIN")) {
                
                points[t1] += 2;
                
            }
            else if(tokens[2].equals("DRAW")) {
                
                points[t1] ++;
                points[t2] ++;
                
            }           
        }
        
        //citesc indicii echipelor cerute
        for(int i = 0; i < nr_queries; i++) {
            
            String str = br.readLine();
            
            queries[i] = Integer.parseInt(str.trim());
            
        }
    }
    
    public void init() {
        
        remaining_games = new int[nr_teams];
        points = new int[nr_teams];
        queries = new int[nr_queries];
        games_to_play = new int[nr_teams][nr_teams];
        
        for(int i = 0; i < nr_teams; i++) {
            
            remaining_games[i] = stages * (nr_teams - 1);
            points[i] = 0;
            
            for(int j = 0; j < nr_teams; j++) {
                
                games_to_play[i][j] = stages;
                
            }
        }
        negative =false;
        
    }
    
    public Graph getGraph(int n) {
        
        int nr_nodes = getLeftNodes() + nr_teams + 1;
        int count = 1;
        Graph g = new Graph(nr_nodes, Graph.Type.DIRECTED);
        
        for(int i = 1; i <= nr_teams - 1; i++) {
            
            for(int j = i + 1; j <= nr_teams; j++) {
                
                if(i != n + 1 && j != n + 1) {
                    
                    int index_i = getIndex(i, n, nr_teams);
                    int index_j = getIndex(j, n, nr_teams);
                    
                    g.addEdge(count, index_i, INF);
                    g.addEdge(count, index_j, INF);
                    g.addEdge(0, count, 2 * games_to_play[i - 1][j - 1]);
                    
                    count ++;
                    
                    if(!g.isEdge(index_i, nr_nodes - 1)) {
                        
                        int score = points[n] - points[i - 1] + 2 * remaining_games[n]; 
                        g.addEdge(index_i, nr_nodes - 1, score);
                        if(points[n] - points[i - 1] + 2 * remaining_games[n] < 0) {
                            
                            negative = true;
                        }
                    }
                    
                    if(!g.isEdge(index_j, nr_nodes - 1)) {
                        
                        int score = points[n] - points[j - 1] + 2 * remaining_games[n];
                        g.addEdge(index_j, nr_nodes - 1, score);
                        if(points[n] - points[j - 1] + 2 * remaining_games[n] < 0) {
                            
                            negative = true;
                        }                       
                    }
                }
            }
        }
        
        return g;
    }
    
    public int getIndex(int i, int target, int teams) {
        
        if(i < target + 1) {
            
            return getLeftNodes() + i;
            
        }
        else return getLeftNodes() + i - 1;
    }
    
    public int getLastNode() {
        
        return getLeftNodes() + nr_teams;
        
    }
    
    public int getLeftNodes() {
        
        if(nr_teams == 3) return 1;
        else return (nr_teams - 2) * (nr_teams - 1) / 2;
    }
}
