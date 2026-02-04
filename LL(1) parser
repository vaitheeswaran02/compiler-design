import java.util.*;

public class LL1ParserTable {

    static Map<Character, List<String>> grammar = new LinkedHashMap<>();
    static Map<Character, Set<String>> first = new LinkedHashMap<>();
    static Map<Character, Set<String>> follow = new LinkedHashMap<>();
    static Map<Character, Map<String, String>> table = new LinkedHashMap<>();
    static Set<String> terminals = new LinkedHashSet<>();
    static char startSymbol;

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        System.out.print("Enter number of productions: ");
        int n = sc.nextInt();
        sc.nextLine();

        System.out.println("Enter productions (use # for epsilon):");
        for (int i = 0; i < n; i++) {
            String p = sc.nextLine();
            char lhs = p.charAt(0);
            grammar.putIfAbsent(lhs, new ArrayList<>());
            String rhs = p.substring(3);
            for (String prod : rhs.split("\\|"))
                grammar.get(lhs).add(prod);
        }

        startSymbol = grammar.keySet().iterator().next();

        findTerminals();
        computeFirst();
        computeFollow();

        // 🔹 OUTPUT FIRST & FOLLOW
        printFirst();
        printFollow();

        buildTable();
        printParsingTable();

        System.out.print("\nEnter input string (end with $): ");
        String input = sc.nextLine();
        parse(input);
    }

    // ================= TERMINALS =================

    static void findTerminals() {
        for (char nt : grammar.keySet()) {
            for (String prod : grammar.get(nt)) {
                for (char c : prod.toCharArray()) {
                    if (!Character.isUpperCase(c) && c != '#')
                        terminals.add(String.valueOf(c));
                }
            }
        }
        terminals.add("$");
    }

    // ================= FIRST =================

    static void computeFirst() {
        for (char nt : grammar.keySet())
            first.put(nt, new LinkedHashSet<>());

        boolean changed;
        do {
            changed = false;
            for (char nt : grammar.keySet()) {
                for (String prod : grammar.get(nt)) {
                    for (int i = 0; i < prod.length(); i++) {
                        char c = prod.charAt(i);
                        if (!Character.isUpperCase(c)) {
                            changed |= first.get(nt).add("" + c);
                            break;
                        }
                        changed |= first.get(nt).addAll(removeEps(first.get(c)));
                        if (!first.get(c).contains("#")) break;
                        if (i == prod.length() - 1)
                            changed |= first.get(nt).add("#");
                    }
                }
            }
        } while (changed);
    }

    // ================= FOLLOW =================

    static void computeFollow() {
        for (char nt : grammar.keySet())
            follow.put(nt, new LinkedHashSet<>());

        follow.get(startSymbol).add("$");

        boolean changed;
        do {
            changed = false;
            for (char nt : grammar.keySet()) {
                for (String prod : grammar.get(nt)) {
                    for (int i = 0; i < prod.length(); i++) {
                        char c = prod.charAt(i);
                        if (Character.isUpperCase(c)) {
                            Set<String> temp = new LinkedHashSet<>();
                            if (i + 1 < prod.length()) {
                                char next = prod.charAt(i + 1);
                                if (Character.isUpperCase(next)) {
                                    temp.addAll(removeEps(first.get(next)));
                                    if (first.get(next).contains("#"))
                                        temp.addAll(follow.get(nt));
                                } else {
                                    temp.add("" + next);
                                }
                            } else {
                                temp.addAll(follow.get(nt));
                            }
                            changed |= follow.get(c).addAll(temp);
                        }
                    }
                }
            }
        } while (changed);
    }

    // ================= PARSING TABLE =================

    static void buildTable() {
        for (char nt : grammar.keySet())
            table.put(nt, new LinkedHashMap<>());

        for (char nt : grammar.keySet()) {
            for (String prod : grammar.get(nt)) {
                Set<String> f = firstOf(prod);
                for (String t : f)
                    if (!t.equals("#"))
                        table.get(nt).put(t, nt + "->" + prod);

                if (f.contains("#"))
                    for (String t : follow.get(nt))
                        table.get(nt).put(t, nt + "->#");
            }
        }
    }

    static Set<String> firstOf(String s) {
        Set<String> res = new LinkedHashSet<>();
        for (char c : s.toCharArray()) {
            if (!Character.isUpperCase(c)) {
                res.add("" + c);
                return res;
            }
            res.addAll(removeEps(first.get(c)));
            if (!first.get(c).contains("#"))
                return res;
        }
        res.add("#");
        return res;
    }

    static Set<String> removeEps(Set<String> s) {
        Set<String> r = new LinkedHashSet<>(s);
        r.remove("#");
        return r;
    }

    // ================= OUTPUT =================

    static void printFirst() {
        System.out.println("\nFIRST SETS:");
        for (char nt : first.keySet())
            System.out.println("FIRST(" + nt + ") = " + first.get(nt));
    }

    static void printFollow() {
        System.out.println("\nFOLLOW SETS:");
        for (char nt : follow.keySet())
            System.out.println("FOLLOW(" + nt + ") = " + follow.get(nt));
    }

    static void printParsingTable() {
        System.out.println("\nLL(1) PARSING TABLE:");
        System.out.print("    ");
        for (String t : terminals)
            System.out.printf("%-8s", t);
        System.out.println();

        for (char nt : table.keySet()) {
            System.out.printf("%-4s", nt);
            for (String t : terminals) {
                String val = table.get(nt).getOrDefault(t, "-");
                System.out.printf("%-8s", val);
            }
            System.out.println();
        }
    }

    // ================= PARSER =================

    static void parse(String input) {
        Stack<String> stack = new Stack<>();
        stack.push("$");
        stack.push("" + startSymbol);
        int i = 0;

        System.out.println("\nSTACK\t\tINPUT\t\tACTION");
        System.out.println("------------------------------------------");

        while (!stack.isEmpty()) {
            String stk = stack.toString();
            String in = input.substring(i);
            String top = stack.peek();
            String curr = "" + input.charAt(i);

            if (top.equals(curr)) {
                stack.pop();
                i++;
                System.out.println(stk + "\t" + in + "\tmatch " + curr);
            } else if (!Character.isUpperCase(top.charAt(0))) {
                System.out.println(stk + "\t" + in + "\tREJECT");
                return;
            } else {
                String prod = table.get(top.charAt(0)).get(curr);
                if (prod == null) {
                    System.out.println(stk + "\t" + in + "\tREJECT");
                    return;
                }
                stack.pop();
                System.out.println(stk + "\t" + in + "\t" + prod);
                String rhs = prod.substring(3);
                if (!rhs.equals("#"))
                    for (int j = rhs.length() - 1; j >= 0; j--)
                        stack.push("" + rhs.charAt(j));
            }
        }
        System.out.println("$\t\t$\t\tACCEPT");
    }
}
