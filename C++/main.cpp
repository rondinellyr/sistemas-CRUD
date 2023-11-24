
// Bibliotecas padrão C++
#include <stdlib.h>
#include <iostream>

// Bibliotecas do conector MySQL C++
#include <jdbc\mysql_connection.h>
#include <jdbc\cppconn\driver.h>
#include <jdbc\cppconn\exception.h>
#include <jdbc\cppconn\prepared_statement.h>

// Biblioteca que achei online para imprimir tabelas
#include <bprinter\table_printer.h> 
using namespace std;

// Autenticação
const string server = "tcp://127.0.0.1:3306";
const string username = "root";
const string password = "";

// Código
int main()
{
    cout << "Aplicativo iniciado com sucesso." << endl;
    system("pause"); // Faz o cmd imprimir "Pressione qualquer tecla para continuar..."
    system("cls"); // Limpa o cmd

    // Essas variáveis não tem valores ainda. Apenas as declaramos para atribuir valor a elas posteriormente
    sql::Driver *driver; // Controlador do SQL
    sql::Connection *con; // Variável que mantém a conexão
    sql::Statement *stmt; // Variável para criarmos queries
    sql::PreparedStatement *pstmt; // Variável para criarmos queries com parâmetros (medida de segurança para impedir injeção de SQL)
    sql::ResultSet *resultSet; // Variável com os dados retornados

    try // tentativa de conexão
    {
        // Obtendo driver
        driver = get_driver_instance();
        // Usando driver para realizar conexão e atribuindo essa conexão a uma variável
        con = driver->connect(server, username, password);
    }
    catch (sql::SQLException e)
    {
        // Mensagem de erro
        cout << "Não foi possível realizar a conexão com o banco de dados. Erro: " << e.what() << endl;
        system("pause");
        exit(1);
    }

    cout << "Iniciando CREATE..." << endl << endl;
    // Criando banco de dados db_prova4bim
    stmt = con->createStatement();
    stmt->execute("CREATE DATABASE IF NOT EXISTS db_prova4bim");
    cout << "Banco de dados db_prova4bim foi criado! (caso inexistente)" << endl;

    // Selecionando banco de dados db_prova4bim
    con->setSchema("db_prova4bim");

    // Apagando tabela aluno caso ela já tenha dados
    stmt = con->createStatement();
    stmt->execute("DROP TABLE IF EXISTS aluno");
    cout << "Tabela aluno foi apagada (caso exista)" << endl;

    // Criando tabela aluno novamente com id PRIMARY KEY, nome, cpf e telefone
    stmt->execute("CREATE TABLE aluno (id INT NOT NULL AUTO_INCREMENT PRIMARY KEY, nome VARCHAR(80) NOT NULL, cpf VARCHAR(50) NOT NULL, telefone VARCHAR(50) NULL);");
    cout << "Tabela aluno criada" << endl;
    delete stmt; // Deletando variável para economizar espaço na memória

    // Inserindo aluno
    cout << "-- Inserindo novos alunos --" << endl;
    pstmt = con->prepareStatement("INSERT INTO aluno(nome, cpf, telefone) VALUES(?,?,?)");
    pstmt->setString(1, "Rondinelly");
    pstmt->setString(2, "673.276.763-54");
    pstmt->setString(3, "40028922");
    pstmt->execute();
    cout << "Aluno Rondinelly registrado" << endl;

    // Inserindo aluno
    pstmt->setString(1, "Monique");
    pstmt->setString(2, "670.234.732-48");
    pstmt->setString(3, "08002929");
    pstmt->execute();
    cout << "Aluna Monique inserida" << endl;

    // Inserindo aluno
    pstmt->setString(1, "Pandia");
    pstmt->setString(2, "155.215.340-55");
    pstmt->setString(3, "12345678");
    pstmt->execute();
    cout << "Aluno Pandia inserido" << endl;

    // Deletando variável para economizar espaço na memória
    delete pstmt;

    cout << "Novos alunos adicionados." << endl;
    system("pause");
    system("cls");

    cout << "Iniciando READ..." << endl << endl;

    // Criando nova query
    stmt = con->createStatement();
    // Executando query SELECT
    resultSet = stmt->executeQuery("SELECT * FROM aluno");

    // Imprimindo cabeçalhos da tabela
    bprinter::TablePrinter tp(&std::cout);
    tp.AddColumn("ID", 5);
    tp.AddColumn("Nome", 25);
    tp.AddColumn("CPF", 20);
    tp.AddColumn("Telefone", 12);
    tp.PrintHeader();
    // Imprimindo todos os itens que recebemos na variável resultSet
    while (resultSet->next())
    {
        // Função da biblioteca bprinter para imprimir os itens que queremos
        tp << resultSet->getInt("id") << resultSet->getString("nome") << resultSet->getString("cpf") << resultSet->getString("telefone");
        
    }
    tp.PrintFooter();

    cout << endl << endl << "READ realizado" << endl;
    system("pause");
    system("cls");
    cout << "Iniciando UPDATE..." << endl << endl;
    cout << "Alterando telefone de Rondinelly para 555888" << endl;

    // Atualizando o telefone do aluno Rondinelly
    pstmt = con->prepareStatement("UPDATE aluno SET telefone = ? WHERE nome = ?");
    pstmt->setString(1, "555888");
    pstmt->setString(2, "Rondinelly");
    pstmt->execute();

    // Criando nova query
    stmt = con->createStatement();
    // Executando query SELECT
    resultSet = stmt->executeQuery("SELECT * FROM aluno");

    // Imprimindo cabeçalhos da tabela
    tp.PrintHeader();
    // Imprimindo todos os itens que recebemos na variável resultSet
    while (resultSet->next())
    {
        // Função da biblioteca bprinter para imprimir os itens que queremos
        tp << resultSet->getInt("id") << resultSet->getString("nome") << resultSet->getString("cpf") << resultSet->getString("telefone");

    }
    tp.PrintFooter();

    cout << endl << endl << "UPDATE concluído." << endl;
    system("pause");
    system("cls");

    cout << "Iniciando DELETE..." << endl << endl;
    cout << "Deletando o aluno Pandia" << endl;

    pstmt = con->prepareStatement("DELETE FROM aluno WHERE nome = ?");
    pstmt->setString(1, "Pandia");
    pstmt->execute();

    // Print the table after deletion
    stmt = con->createStatement();
    resultSet = stmt->executeQuery("SELECT * FROM aluno");

    // Imprimindo cabeçalhos da tabela
    tp.PrintHeader();
    // Imprimindo todos os itens que recebemos na variável resultSet
    while (resultSet->next()) {
        tp << resultSet->getInt("id") << resultSet->getString("nome") << resultSet->getString("cpf") << resultSet->getString("telefone");
    }
    tp.PrintFooter();

    cout << endl << endl << "DELETE realizado" << endl;

    cout << endl << endl << "Terminamos!" << endl;
    system("pause");
    // Apagando variáveis inutilizadas
    delete resultSet;
    delete stmt;
    delete con;
    return 0;
}