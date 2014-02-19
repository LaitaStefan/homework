//Papa Florin, 324CA

import java.util.Arrays;
import java.util.Vector;

public class Pitch {
    Directions dir[][];
    Coord currentPoz;
    int[][] values;
    
    public Pitch() {
        dir = new Directions[13][9];
        values = new int[13][9];
        int ways[] = new int[8];
        Arrays.fill(ways,1);
        currentPoz = new Coord(6, 4);
        
        for(int i = 0; i < 13 ; i++) {
            for(int j = 0; j < 9; j++) {
                dir[i][j] = new Directions(ways);
                values[i][j] = 13 - i;
                if(j == 0) {       
                    for(int k = 4; k <= 7; k++) {
                        dir[i][j].removeDirection(k);
                    }
                    dir[i][j].removeDirection(0);
                    dir[i][j].visited = 1;
                }
                
                if(j == 8) {
                    for(int k = 0; k <= 4; k++) {
                        dir[i][j].removeDirection(k);
                    }
                    dir[i][j].visited = 1;
                }
                
                if (i == 0) {
                    int aux[] = new int[8];
                    Arrays.fill(aux,0);
                    dir[i][j] = new Directions(aux);
                    dir[i][j].visited = 1;
                }
                
                if (i == 1) {
                    
                    dir[i][j].removeDirection(6);
                    dir[i][j].removeDirection(7);
                    dir[i][j].visited = 1;
                    
                    for(int k = 0; k <= 2; k++) {
                        dir[i][j].removeDirection(k);
                    }
                    dir[i][j].visited = 1;
                    
                    if(j == 3) {
                        dir[i][j].addDirections(1);
                        dir[i][j].addDirections(2);
                    }
                    
                    if(j == 4) {
                        dir[i][j] = new Directions(ways);
                        dir[i][j].visited = 0;
                    }
                    
                    if(j == 5) {
                        dir[i][j].addDirections(6);
                        dir[i][j].addDirections(7);
                    } 
                }
                
                if(i == 11) {
                    for(int k = 2; k <= 6; k++) {
                        dir[i][j].removeDirection(k);
                    }
                    dir[i][j].visited = 1;
                    
                    if(j == 3) {
                        dir[i][j].addDirections(2);
                        dir[i][j].addDirections(3);
                    }
                    
                    if(j == 4) {
                        dir[i][j] = new Directions(ways);
                        dir[i][j].visited = 0;
                    }
                    
                    if(j == 5) {
                        dir[i][j].addDirections(5);
                        dir[i][j].addDirections(6);
                    }
                }
                
                if(i == 12) {
                    int aux[] = new int[8];
                    Arrays.fill(aux,0);
                    dir[i][j] = new Directions(aux);
                    dir[i][j].visited = 1;
                }
            }
        }
    }
    
    public int eval() {
        return values[currentPoz.x][currentPoz.y];
    }
    
    public Pitch(Directions d[][], Coord cpz) {
        dir = new Directions[13][9];
        values = new int[13][9];
                
        for(int i = 0; i < 13; i++) {
            for(int j = 0; j < 9; j++) {
                dir[i][j] = d[i][j].clone();
                values[i][j] = 13 - i;
            }
        }
        currentPoz = new Coord(cpz.x, cpz.y);
    }
    
    public Coord move(int direction) { //intoarce noile coordonate
        dir[currentPoz.x][currentPoz.y].visited = 1;

        switch(direction) {
            case 0:
                dir[currentPoz.x][currentPoz.y].removeDirection(0);
                dir[currentPoz.x-1][currentPoz.y].removeDirection(4);
                currentPoz = new Coord(currentPoz.x-1,currentPoz.y);
                return new Coord(currentPoz.x,currentPoz.y);
            case 1:
                dir[currentPoz.x][currentPoz.y].removeDirection(1);
                dir[currentPoz.x-1][currentPoz.y+1].removeDirection(5);
                currentPoz = new Coord(currentPoz.x-1,currentPoz.y+1);
                return new Coord(currentPoz.x,currentPoz.y);
            case 2:
                dir[currentPoz.x][currentPoz.y].removeDirection(2);
                dir[currentPoz.x][currentPoz.y+1].removeDirection(6);
                currentPoz = new Coord(currentPoz.x,currentPoz.y+1);
                return new Coord(currentPoz.x, currentPoz.y);
            case 3:
                dir[currentPoz.x][currentPoz.y].removeDirection(3);
                dir[currentPoz.x+1][currentPoz.y+1].removeDirection(7);
                currentPoz = new Coord(currentPoz.x+1,currentPoz.y+1);
                return new Coord(currentPoz.x,currentPoz.y);
            case 4:
                dir[currentPoz.x][currentPoz.y].removeDirection(4);
                dir[currentPoz.x+1][currentPoz.y].removeDirection(0);
                currentPoz = new Coord(currentPoz.x+1,currentPoz.y);
                return new Coord(currentPoz.x,currentPoz.y);
            case 5:
                dir[currentPoz.x][currentPoz.y].removeDirection(5);
                dir[currentPoz.x+1][currentPoz.y-1].removeDirection(1);
                currentPoz = new Coord(currentPoz.x+1,currentPoz.y-1);
                return new Coord(currentPoz.x,currentPoz.y);
            case 6:
                dir[currentPoz.x][currentPoz.y].removeDirection(6);
                dir[currentPoz.x][currentPoz.y-1].removeDirection(2);
                currentPoz = new Coord(currentPoz.x,currentPoz.y-1);
                return new Coord(currentPoz.x,currentPoz.y);
            case 7:
                dir[currentPoz.x][currentPoz.y].removeDirection(7);
                dir[currentPoz.x-1][currentPoz.y-1].removeDirection(3);
                currentPoz = new Coord(currentPoz.x-1,currentPoz.y-1);
                return new Coord(currentPoz.x,currentPoz.y);
            default: return null;  
        }
    }
    
    public Coord multipleMoves(String[] mv) {
        
        for(int i = 2; i < mv.length; i++ ) {
             currentPoz = move(Integer.parseInt(mv[i].trim()));
        }
        return currentPoz;
    }
    
    public boolean visited() {
        if(dir[currentPoz.x][currentPoz.y].visited == 1) return true;
        else return false;
    }
    
    public int gameOver() {
        if(currentPoz.x == 0 && currentPoz.y <= 5 && currentPoz.y >= 3) {
            return 1;
        }
        if(currentPoz.x == 12 && currentPoz.y <= 5 && currentPoz.y >= 3) {
            return -1;
        }
        return 0;
    }
    
    public boolean noMoreMoves() {
        int count = 0;
        for(int i = 0; i < 8; i++) {
            if(dir[currentPoz.x][currentPoz.y].direction[i] == 1) count++;
        }
        if(count == 0) return true;
        return false;
    }
    
    public Vector<Integer> getPossibleDirections() {
        return dir[currentPoz.x][currentPoz.y].getPossibleDirections();
    }
}
