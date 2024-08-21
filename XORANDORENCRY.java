package experiment2;

import java.util.Scanner;

public class operation {

    public static void main(String[] args) {

        // Create a Scanner object to take input from the user
        Scanner scanner = new Scanner(System.in);

        // Prompt the user to enter a string
        System.out.print("Enter a string: ");
        String original = scanner.nextLine();

        // Create StringBuilder objects to store the results
        StringBuilder andResult = new StringBuilder();
        StringBuilder orResult = new StringBuilder();
        StringBuilder xorResult = new StringBuilder();

        // Iterate through each character in the user input string
        for (char ch : original.toCharArray()) {
            System.out.print(ch + ":");

            int charValue = (int) ch;
            System.out.print(charValue + ":");

            int andChar = charValue & 127;
            System.out.print(andChar + ":");
            char result1 = (char) andChar;

            int orChar = charValue | 127;
            System.out.print(orChar + ":");
            char result2 = (char) orChar;

            int xorChar = charValue ^ 127;
            System.out.print(xorChar);
            char result3 = (char) xorChar;

            System.out.println("\n");

            // Append the results to the respective StringBuilder objects
            andResult.append(result1);
            orResult.append(result2);
            xorResult.append(result3);
        }

        // Print the original and the resulting strings after bitwise operations
        System.out.println("Original String: " + original);
        System.out.println("AND with 127: " + andResult.toString());
        System.out.println("OR with 127: " + orResult.toString());
        System.out.println("XOR with 127: " + xorResult.toString());

        // Close the scanner
        scanner.close();
    }
}
