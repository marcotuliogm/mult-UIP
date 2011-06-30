/**
 * @file menu_cli.h
 *
 * @brief Arquivo com as listas de funcoes habilitadas para o usuario da CLI poder usar.
 *
 * @author Marco Tulio Martins
 */
/**
 * @brief Lista com todas as funcoes da CLI
 *
 * Lista com os parametros necessarios para as funcoes da CLI
 *
 */
cli_str const api_cli[] =
{
	{do_get_temperature, "TEMPERATURE","Retorna a temperatura da placa."},
	{do_IP, "IP","Sintaxe -[SET IP <address>] COnfigura endereco de IP."},
	{do_gateway, "GATEWAY","Sintaxe -[SET GATEWAY <address>] Configura endereco de Gateway."},
	{do_mask, "MASCARA","Sintaxe -[SET MASK <address>] Configura mascara de rede."},
	{do_configIp, "IPCONFIG","Retorna configuração da rede."},
	{do_MAC, "MAC","Retorna MAC da placa."},
	{do_init, "INIT","Mensagem inicial."},
	{do_lcd, "LCD","Sintaxe -[SET LCD <data>] "},
	{do_chat, "CHAT","CHAT PORTA troca de mensagens."},
	//{do_ftp, "FTP", "FTP [END IP] - ftp client"},
	//inserir outra função. Vai... <func tratadora>, "NOME",<mensage de help.>;
	{NULL, NULL},
};
