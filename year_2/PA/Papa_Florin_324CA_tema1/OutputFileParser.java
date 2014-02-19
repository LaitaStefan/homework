//Papa Florin, 324CA

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Vector;

public class OutputFileParser {
    BufferedWriter out;
    public OutputFileParser(String file, Integer m, Vector<Integer> v) {
        try {
            out = new BufferedWriter(new FileWriter(file));
            writeToFile(out, m, v);
            out.close();
        }
        catch(IOException ex) {
            System.out.println("Eroare la scrierea in fisier");
        }
    }
    
    public void writeToFile(BufferedWriter bw, Integer m, Vector<Integer> v) {
        try {
            bw.write(m.toString());
            bw.newLine();
            Integer aux = v.size();
            bw.write(aux.toString());
            bw.newLine();
            for(int i = 0; i < v.size(); i++) {
                bw.write(v.get(i).toString());
                bw.newLine();
            }
        }
        catch(IOException ex) {
            System.out.println("Eroare la scrierea in fisier");
        }
    }   
}
