package fr.test.exportExcel;


import java.io.FileWriter;
import java.util.ArrayList;
import java.util.Arrays;

public class Main {
    public static void main(String[] args) throws Exception{
        ArrayList resultTab = new ArrayList();
        MySQLAccess dao = new MySQLAccess();
        resultTab = dao.readDataBase();
        String filePath = "test.csv";
        FileWriter writer = new FileWriter(filePath);
        String id_ticket ;
        String subject ;
        String description ;
        String status ;
        for (int i = 0; i< resultTab.size();i+=4){
            id_ticket = (String)resultTab.get(i);
            subject =  (String)resultTab.get(i+1);
            description = (String) resultTab.get(i+2);
            status =  (String)resultTab.get(i+3);
            CSVUtils.writeLine(writer, Arrays.asList(id_ticket,subject,description,status));
        }
        writer.flush();
        writer.close();

    }



}
