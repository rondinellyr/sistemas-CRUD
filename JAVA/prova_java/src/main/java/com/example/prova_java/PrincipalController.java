package com.example.prova_java;

import java.math.BigDecimal;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestParam;

import java.sql.Date;

import org.json.JSONObject;
import org.json.JSONArray;

@Controller
public class PrincipalController {
    private JSONArray matrizResultados = new JSONArray();
    @GetMapping(value = "/")
    public String getIndex(Model model) {
        model.addAttribute("arrayDados", matrizResultados.toString());
        return "index";
    }

    private static Connection conectar() {
        String jdbcUrl = "jdbc:mysql://localhost:3306/animal_java";
        String usuario = "root";
        String senha = "pegadaecologica";

        try {
            return DriverManager.getConnection(jdbcUrl, usuario, senha);
        } catch (SQLException e) {
            e.printStackTrace();
            return null;
        }
    }

    @PostMapping("/adicionar")
    public String adicionarAnimal(@RequestParam String nome, @RequestParam String raca, @RequestParam String sexo,
            @RequestParam String cor, @RequestParam String data, @RequestParam String peso, @RequestParam String altura,
            Model model) {

        try (Connection conexao = conectar()) {
            String sql = "INSERT INTO animal (nome, raca, sexo, cor, nascimento, peso, altura) VALUES (?, ?, ?, ?, ?, ?, ?)";
            PreparedStatement pstmt = conexao.prepareStatement(sql);

            pstmt.setString(1, nome);
            pstmt.setString(2, raca);
            pstmt.setString(3, sexo);
            pstmt.setString(4, cor);
            pstmt.setDate(5, consertarData(data));
            pstmt.setBigDecimal(6, new BigDecimal(peso));
            pstmt.setBigDecimal(7, new BigDecimal(altura));

            int linhasAfetadas = pstmt.executeUpdate();

            if (linhasAfetadas > 0) {
                System.out.println("Animal adicionado com sucesso!");
            } else {
                System.out.println("Falha ao adicionar animal.");
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return "redirect:/";
    }

    @PostMapping("/atualizar")
    public String atualizarAnimal(@RequestParam String nome, @RequestParam String raca, @RequestParam String sexo,
            @RequestParam String cor, @RequestParam String data, @RequestParam String peso, @RequestParam String altura,
            @RequestParam String nome_substituicao, Model model) {

        try (Connection conexao = conectar()) {
            String sql = "UPDATE animal SET nome = ?, raca = ?, sexo = ?, cor = ?, nascimento = ?, peso = ?, altura = ? WHERE nome ='"
                    + nome_substituicao + "'";
            PreparedStatement pstmt = conexao.prepareStatement(sql);

            pstmt.setString(1, nome);
            pstmt.setString(2, raca);
            pstmt.setString(3, sexo);
            pstmt.setString(4, cor);
            pstmt.setDate(5, consertarData(data));
            pstmt.setBigDecimal(6, new BigDecimal(peso));
            pstmt.setBigDecimal(7, new BigDecimal(altura));

            int linhasAfetadas = pstmt.executeUpdate();

            if (linhasAfetadas > 0) {
                System.out.println("Animal adicionado com sucesso!");
            } else {
                System.out.println("Falha ao adicionar animal.");
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return "redirect:/";
    }

    @PostMapping("/deletar")
    public String deletarAnimal(@RequestParam String nome, Model model) {
        try (Connection conexao = conectar()) {
            String sql = "DELETE FROM animal WHERE nome ='" + nome + "'";
            PreparedStatement pstmt = conexao.prepareStatement(sql);
            int linhasAfetadas = pstmt.executeUpdate();

            // Verificar se a exclusão foi bem-sucedida
            if (linhasAfetadas > 0) {
                System.out.println("Exclusão bem-sucedida!");
            } else {
                System.out.println("Nenhuma linha foi excluída. Verifique a condição WHERE.");
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return "redirect:/";
    }

    @PostMapping("/mostrar")
    public String mostrarAnimais(Model model) {
        try (Connection conexao = conectar()) {
            String sql = "SELECT * FROM animal";
            PreparedStatement pstmt = conexao.prepareStatement(sql);
            ResultSet resultSet = pstmt.executeQuery();
            matrizResultados  = new JSONArray();
            // Processar os resultados
            while (resultSet.next()) {
                // Obter dados de cada coluna
                int id = resultSet.getInt("id");
                String nome = resultSet.getString("nome");
                String raca = resultSet.getString("raca");
                String sexo = resultSet.getString("sexo");
                String cor = resultSet.getString("cor");
                String nascimento = resultSet.getString("nascimento");
                double peso = resultSet.getDouble("peso");
                double altura = resultSet.getDouble("altura");
                JSONObject jsonRow = new JSONObject();
                jsonRow.put("id", id);
                jsonRow.put("nome", nome);
                jsonRow.put("raca", raca);
                jsonRow.put("sexo", sexo);
                jsonRow.put("cor", cor);
                jsonRow.put("nascimento", nascimento);
                jsonRow.put("peso", peso);
                jsonRow.put("altura", altura);

                // Adicionar o objeto JSON à array JSON
                matrizResultados.put(jsonRow);
            }

        } catch (SQLException e) {
            e.printStackTrace();
        }
        return "redirect:/";
    }

    private static Date consertarData(String dataBrasileira) {
        String[] partes = dataBrasileira.split("/");

        String dia = partes[0];
        String mes = partes[1];
        String ano = partes[2];

        String dataSqlString = ano + "-" + mes + "-" + dia;

        Date dataSql = Date.valueOf(dataSqlString);

        return dataSql;
    }
}