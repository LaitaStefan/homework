//Papa Florin, 324CA

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.StringTokenizer;
import java.util.Vector;

@SuppressWarnings("unchecked")
public class InputFileParser {
    private BufferedReader br;
    private Integer N,M,L;
    private Vector<Integer> fence = new Vector();
    public InputFileParser(String s) {
        try {
            br = new BufferedReader(new FileReader(s));
            try {
                String line = br.readLine();
                getNML(line);
                buildFence(br);
                br.close();
            }
            catch(IOException ex) {
                System.out.println("Fisierul de intrare nu a putut fi citit");
            }
        }
        catch(FileNotFoundException e) {
            System.out.println("Fisierul de intrare nu a putut fi citit");
        }
    }
    
    Integer getN() {
        return N;
    }
    
    Integer getM() {
        return M;
    }
    
    Integer getL() {
        return L;
    }
    
    Vector<Integer> getFence() {
        return fence;
    } 
    
    public void getNML(String s) {
        StringTokenizer tok = new StringTokenizer(s," ");
        N = Integer.parseInt(tok.nextToken());
        M = Integer.parseInt(tok.nextToken());
        L = Integer.parseInt(tok.nextToken());
    }
    
    void buildFence(BufferedReader r) {
        for(int i=0; i<N; i++) {
            try {
                String line = br.readLine();
                fence.add(Integer.parseInt(line.trim()));
            }
            catch(IOException ex) {
                System.out.println("Fisierul de intrare nu a putut fi citit");
            }
        }
    }
}
