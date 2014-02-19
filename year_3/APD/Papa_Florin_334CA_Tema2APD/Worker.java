//Papa Florin, 334CA

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.Enumeration;
import java.util.StringTokenizer;
import java.util.concurrent.ConcurrentHashMap;

public class Worker extends Thread {
    WorkPool wp;
    

    public Worker(WorkPool workpool) {
            this.wp = workpool;
    }
    
    public boolean starts(String s, String delim) {
            for(int i = 0; i < delim.length(); i++) {
                if(s.startsWith(delim.substring(i, i + 1))) return true;
            }
            return false;
        }
        
    public boolean ends(String s, String delim) {
        for(int i = 0; i < delim.length(); i++) {
            if(s.endsWith(delim.substring(i, i + 1))) return true;
        }
        return false;
    }
    
    //operatia de map
    void processMapSolution(PartialSolution ps) {
        try {
            ConcurrentHashMap<String, Integer> hash = new ConcurrentHashMap();
            String s = "";
            String delim = "\t\n\r\f,. :;!?()-\'\"\\";
            int middle = 0, init = 0;
            BufferedReader br = new BufferedReader(new FileReader(ps.file));
            int start = ps.start;
            int stop = ps.stop;       
            if(start > 0) {
                start--;
                init = 1;
            }
            char content[] = new char[stop - start + 1];
            br.skip(start); 
            int success = br.read(content);
            String contents = new String(content, 0, success);
            
            StringTokenizer tok;
            if(init == 1) {
                String aux = contents.substring(1,contents.length() - 1);
                tok = new StringTokenizer(aux, delim);
            }
            else {
                String aux = contents.substring(0,contents.length() - 1);
                tok = new StringTokenizer(aux, delim);
            }
            
            //primul token
            String aux2 = contents.substring(1, contents.length());
            boolean first = starts(contents, delim);
            boolean second = starts(aux2, delim);
            if(tok.hasMoreTokens()) {
                s = tok.nextToken().toLowerCase();
                if(first || second || start == 0) {
                    if(hash.get(s) != null) {
                        int frecv = hash.get(s);
                        hash.put(s, frecv + 1);
                    }
                    else hash.put(s, 1);
                }
            }
            
            //tokenii din mijloc
            while(tok.hasMoreTokens()) {             
                s = tok.nextToken().toLowerCase();
                middle = 1;
                if(tok.hasMoreTokens()) {               
                    if(hash.get(s) != null) {
                        int frecv = hash.get(s);
                        hash.put(s, frecv + 1);
                    }
                    else hash.put(s, 1);
                }                  
            }
            
            //daca ultimul cuvant se termina cu un delimitator, nu citim
            //suplimentar din fisier
            aux2 = contents.substring(0, contents.length() - 1);
            boolean last = ends(contents, delim);
            boolean second_to_last = ends(aux2, delim);
            if((last || second_to_last) && middle == 1) {
                if(hash.get(s) != null) {
                    int frecv = hash.get(s);
                    hash.put(s, frecv + 1);
                }
                else hash.put(s, 1);
            }
            else if(middle == 1 && !second_to_last && !last) {//mai citesc
                while(true) {
                    char extra[] = new char[10];
                    int test = br.read(extra);
                    if(test != -1) {
                        String aux;
                        s += new String(extra, 0, test).toLowerCase();
                        StringTokenizer t = new StringTokenizer(s, delim);
                        aux = t.nextToken().toLowerCase();
                        if(aux.length() < s.length()) {
                            if(hash.get(aux) != null) {
                                int frecv = hash.get(aux);
                                hash.put(aux, frecv + 1);
                            }
                            else hash.put(aux, 1);
                            break;
                        }
                    }
                    else {
                        if(hash.get(s) != null) {
                            int frecv = hash.get(s);
                            hash.put(s, frecv + 1);
                        }
                        else hash.put(s, 1);
                        break;
                    }
                }
            }
            br.close();
            
            wp.putSolution(hash, ps.file);
        } catch (IOException ex) {
            System.out.println("File " + ps.file +  " was not found");
        }
    }
    
    void processReduceSolution(PartialSolution ps) {
        Enumeration e = ps.pair.hash.get(0).keys();
        while(e.hasMoreElements()) {
            String s = (String)e.nextElement();
            int val = ps.pair.hash.get(0).get(s);
            wp.putValues(s, val, ps.file);           
        }
    }
    
    void processFinalSolution(PartialSolution ps) {
        int target = wp.getTarget();
        if(ps.index != target) {
            float s = 0;
            Enumeration e = ps.pair.hash.get(0).keys();
            while(e.hasMoreElements()) {
                String st = (String)e.nextElement();
                float val = ps.pair.hash.get(0).get(st); //frecventa cheii "st"
                ConcurrentHashMap<String, Integer> h = wp.getMap(target);
                if(h.get(st) != null) {
                    float frecv = h.get(st);
                    float targetTotal = wp.getTotals(target);
                    float currentTotal = wp.getTotals(ps.index);
                    s += (val/currentTotal) * (frecv/targetTotal);
                }
            }
            wp.setResemblance(ps.index, s * 100);
        }
    }

    void processPartialSolution(PartialSolution ps) {
        if(ps.type.equals("m")) processMapSolution(ps);
        else if(ps.type.equals("r")) processReduceSolution(ps);
        else if(ps.type.equals("a")) processFinalSolution(ps);
    }

    public void run() {
        while (true) {
                PartialSolution ps = wp.getWork();
                if (ps == null)
                        break;

                processPartialSolution(ps);
        }
    }
}
