//Papa Florin, 324CA

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.StringTokenizer;
import java.util.Vector;

@SuppressWarnings("unchecked")
public class InputFileParser2 {
    private BufferedReader br;
    private Integer size,elem_nr;
    private Vector<Integer> elem = new Vector();
    public InputFileParser2(String s) {
        try {
            br = new BufferedReader(new FileReader(s));
            try {
                String line = br.readLine();
                get_first_line(line);
                read_elem(br);
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
    
    Integer getSize() {
        return size;
    }
    
    Integer getElemNr() {
        return elem_nr;
    }
    
    Vector<Integer> getElem() {
        return elem;
    } 
    
    public void get_first_line(String s) {
        StringTokenizer tok = new StringTokenizer(s," ");
        size = Integer.parseInt(tok.nextToken());
        elem_nr = Integer.parseInt(tok.nextToken());
    }
    
    void read_elem(BufferedReader r) {
        for(int i=0; i<size; i++) {
            try {
                String line = br.readLine();
                elem.add(Integer.parseInt(line.trim()));
            }
            catch(IOException ex) {
                System.out.println("Fisierul de intrare nu a putut fi citit");
            }
        }
    }
}
