//Papa Florin, 334CA

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.text.DecimalFormat;
import java.util.concurrent.ConcurrentHashMap;
import java.math.RoundingMode;

public class FileParser {
    
    static void addToWorkPool(String file, int D, WorkPool wp) {
        File f = new File(file);
        long size = f.length();
        
        for(int i = 0; i < size; i = i + D) {
            int stop = i + D - 1;
            if(stop > size) stop = (int)size - 1;
            wp.putWork(new PartialSolution(file, i, stop));
        }
    } 
    
    public static void main(String arg[]) throws FileNotFoundException, 
                                                IOException, 
                                                InterruptedException {
        
        int NT = Integer.parseInt(arg[0]); //numarul de threaduri
        String doc;
        int D;          //dimensiunea citita
        float border;   //pragul de similaritate
        int ND;         //numarul de documente
        WorkPool wp_phase1 = new WorkPool(NT);	//folosit la map
        WorkPool wp_phase2 = new WorkPool(NT);	//folosit la reduce
        Worker w[] = new Worker[NT];
        
        BufferedReader br = new BufferedReader(new FileReader(arg[1]));
        doc = br.readLine().trim();
        
        D = Integer.parseInt(br.readLine().trim());
        border = Float.parseFloat(br.readLine().trim());
        ND = Integer.parseInt(br.readLine().trim());

        for(int i = 0; i < ND; i++) {
            String s = br.readLine().trim();
            addToWorkPool(s, D, wp_phase1);
        }

        br.close();
        
        //obtinere rezultate partiale
        for(int i = 0; i < NT; i++) {
            w[i] = new Worker(wp_phase1);
            w[i].start();
        }
        for(int i = 0; i < NT; i++) {
            w[i].join();
        }
        
        for(int i = 0; i < wp_phase1.result.size(); i++) {
            int initial = 0; //primul element il depun direct in result
            while(wp_phase1.result.get(i).hash.size() > 0) {
                String f = wp_phase1.result.get(i).file;
                ConcurrentHashMap<String, Integer> hash, hash2;
                hash2 = wp_phase1.result.get(i).hash.get(0);
                hash = new ConcurrentHashMap(hash2);
                MyPair p = new MyPair(f, hash);
                if(initial == 0) {
                    wp_phase2.result.add(p);
                    initial = 1;
                }
                else {
                    wp_phase2.putWork(new PartialSolution(p));
                }
                wp_phase1.result.get(i).hash.remove(0);
            }
        }

        //obtinere frecvente
        for(int i = 0; i < NT; i++) {
            w[i] = new Worker(wp_phase2);
            w[i].start();
        }
        for(int i = 0; i < NT; i++) {
            w[i].join();
        }
        
        //calcul asemanari dupa formula din enuntul temei
        wp_phase2.getTotal();
        wp_phase2.set_target_index(doc);
        for(int i = 0; i < wp_phase2.totals.size(); i++) {
            PartialSolution ps;
            ps = new PartialSolution(wp_phase2.result.get(i), "a", i);
            wp_phase2.putWork(ps);         
        }
        
        for(int i = 0; i < NT; i++) {
            w[i] = new Worker(wp_phase2);
            w[i].start();
        }
        for(int i = 0; i < NT; i++) {
            w[i].join();
        }
        
        wp_phase2.sortResemblance();
        BufferedWriter bw = new BufferedWriter(new FileWriter(arg[2]));
        bw.write("Rezultate pentru: " + "(" + doc + ")");
        bw.newLine();
        bw.newLine(); 
        for(int i = 0; i < wp_phase2.result.size(); i++) {
            if(wp_phase2.resemblance[i] > border) {
                DecimalFormat df = new DecimalFormat();
                df.setMaximumFractionDigits(3);
                df.setMinimumFractionDigits(3);
                df.setRoundingMode(RoundingMode.HALF_DOWN); //pentru trunchiere

                String s = wp_phase2.result.get(i).file + " ";
                s += "(" + df.format(wp_phase2.resemblance[i]) + "%)";
                bw.write(s);
                bw.newLine();
            }
        }
        bw.close();
    }
}    
    
