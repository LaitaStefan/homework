//Papa Florin, 324CA

import java.util.Vector;

@SuppressWarnings("unchecked")
public class Directions {
    int visited = 0;
    public int direction[] = new int[8]; //este 1 cand calea este valida, 0 altfel
    
    public Directions(int ways[]) {
        for(int i = 0; i < 8; i++) {
            direction[i] = ways[i];
        }
    }
    
    public void setVisited() {
        visited = 1;
    }
    
    public Vector<Integer> getPossibleDirections() {
        Vector<Integer> v = new Vector();
        for(int i = 0; i < 8; i++ ) {
            if(direction[i] == 1) {
                v.add(i);
            }
        }
        return v;
    }
    
    public void removeDirection(int d) {
        direction[d] = 0;
    }
    
    public void addDirections(int d) {
        direction[d] = 1;
    }
    
    public Directions clone() {
        Directions aux = new Directions(direction);
        aux.visited = visited;
        return aux;
    }
}
