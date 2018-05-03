package fr.test.exportExcel;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;



public class MySQLAccess {
    private Connection connect = null;
    private Statement statement = null;
    private PreparedStatement preparedStatement = null;
    private ResultSet resultSet = null;

    public ArrayList readDataBase() throws Exception {
        ArrayList result = new ArrayList();
        try {
            // This will load the MySQL driver, each DB has its own driver
            Class.forName("com.mysql.jdbc.Driver");
            // Setup the connection with the DB
            connect = DriverManager
                    .getConnection("jdbc:mysql://localhost/worknshare?"
                            + "user=root&autoReconnect=true&useSSL=false");

            // Statements allow to issue SQL queries to the database
            statement = connect.createStatement();
            // Result set get the result of the SQL query
            resultSet = statement.executeQuery("select * from ticket");
            result = writeResultSet(resultSet);

        } catch (Exception e) {
            throw e;
        } finally {
            close();
        }
        return result;

    }

    private ArrayList writeResultSet(ResultSet resultSet) throws SQLException {
        // ResultSet is initially before the first data set
         ArrayList resultTab = new ArrayList();
        while (resultSet.next()) {

            String id_ticket = resultSet.getString("id_ticket");
            String subject = resultSet.getString("subject");
            String description = resultSet.getString("description");
            String status = resultSet.getString("state");

            resultTab.add(id_ticket);
            resultTab.add(subject);
            resultTab.add(description);
            if (status.equals("0"))
                resultTab.add("unsolved");
            else
                resultTab.add("solved");

        }

        return resultTab;
    }

    private void close() {
        try {
            if (resultSet != null) {
                resultSet.close();
            }

            if (statement != null) {
                statement.close();
            }

            if (connect != null) {
                connect.close();
            }
        } catch (Exception e) {

        }
    }

}
