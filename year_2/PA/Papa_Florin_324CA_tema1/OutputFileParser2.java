//Papa Florin, 324CA

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Vector;

@SuppressWarnings("unchecked")
public class OutputFileParser2 {
    BufferedWriter out;
    public OutputFileParser2(String file, Integer sum, Integer nr_interv, Vector<Integer> v) {
        try {
            out = new BufferedWriter(new FileWriter(file));
            writeToFile(out, sum, nr_interv,v);
            out.close();
        }
        catch(IOException ex) {
            System.out.println("Eroare la scrierea in fisier");
        }
    }
    
    public void writeToFile(BufferedWriter bw, Integer sum, Integer nr_interv, Vector<Integer> v) {
        try {
            bw.write(sum.toString());
            //System.out.println(m);
            bw.newLine();
            //Integer aux = v.size();
            bw.write(nr_interv.toString());
            //System.out.println(v.size());
            bw.newLine();
            int i=v.size()-2;
            while(i>=0) {
                bw.write(v.get(i).toString()+" "+v.get(i+1).toString());
                //System.out.println(v.get(i));
                bw.newLine();
                i-=2;
            }
        }
        catch(IOException ex) {
            System.out.println("Eroare la scrierea in fisier");
        }
    }   
}
