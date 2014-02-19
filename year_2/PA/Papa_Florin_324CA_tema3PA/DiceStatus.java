//Papa Florin, 324CA

public class DiceStatus {
    
    int faces[];
    int sum;        //distanta de la sursa la zarul curent
    int index;     //numarul de secventa in cadrul zarurilor posibile
    int x, y;       //coordonatele nodului
    
    public DiceStatus() {
        
        faces = new int[6];
        sum = 10000;
    }
    
    public int[] moveDice(int direction) {
        
        int aux[] = new int[6];
        
        //sus
        if(direction == 0) {
            
            aux[0] = faces[1];
            aux[1] = faces[5];
            aux[2] = faces[2];
            aux[3] = faces[3];
            aux[4] = faces[0];
            aux[5] = faces[4];
            
            return aux;
        }
        //dreapta
        else if(direction == 1) {
            
            aux[0] = faces[3];
            aux[1] = faces[1];
            aux[2] = faces[0];
            aux[3] = faces[5];
            aux[4] = faces[4];
            aux[5] = faces[2];
            
            return aux;
        }
        //jos
        else if(direction == 2) {
            
            aux[0] = faces[4];
            aux[1] = faces[0];
            aux[2] = faces[2];
            aux[3] = faces[3];
            aux[4] = faces[5];
            aux[5] = faces[1];
            
            return aux;
        }
        //stanga
        else {
            
            aux[0] = faces[2];
            aux[1] = faces[1];
            aux[2] = faces[5];
            aux[3] = faces[0];
            aux[4] = faces[4];
            aux[5] = faces[3];
            
            return aux;
        }
    }
    
    //egalitatea este data de starea zarului (fete identice)
    public boolean equals(DiceStatus d) {
        
        int count = 0;
        
        for(int i = 0; i < 6; i++) {
            
            if(faces[i] != d.faces[i]) count++;
        }
        
        return count == 0;
    }
}
