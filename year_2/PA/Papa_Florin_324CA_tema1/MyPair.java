//Papa Florin, 324CA

public class MyPair implements Comparable{
    Integer start;
    Integer end;
    
    public MyPair(Integer a,Integer b) {
        start = a;
        end = b;
    }
    
    public int compareTo(Object a) {
        MyPair p = (MyPair)a;
        return -start.compareTo(p.start); //sortare descrescatoare
    }
}
