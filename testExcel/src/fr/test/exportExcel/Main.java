package fr.test.exportExcel;


import java.io.FileWriter;
import java.util.ArrayList;

public class Main {
    public static void main(String[] args) throws Exception{
        ArrayList resultTab = new ArrayList();
        MySQLAccess dao = new MySQLAccess();
        resultTab = dao.readDataBase();
        String filePath = "D:\\Dossier de stephane\\cours\\2i\\Projannu.2\\testExcel\\test.csv";
        FileWriter writer = new FileWriter(filePath);
        CSVUtils.writeLine(writer, resultTab);
        writer.flush();
        writer.close();

    }



}
