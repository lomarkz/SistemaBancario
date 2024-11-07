#define _WIN32_WINNT 0x0601

#include<stdio.h> //Lib b�sica
#include<stdlib.h> //Lib B�sica
#include<locale.h> //Setar o local para portugu�s
#include<string.h> //Lib b�sica
#include<windows.h> //Lib para pegar func�es e menu do windows
#include<time.h> //Lib para manipular data e hora

/*Declara��o de structs*/
typedef struct SUltimosSaques {

	float valor;
	int dia,mes,ano,hora,minuto,segundo; //Para Guardar a data, guardo cada valor em cada vari�vel
	long numeroConta;

} TUltimosSaques;

typedef struct SUltimosDepositos {

	float valor;
	int dia,mes,ano,hora,minuto,segundo; //Para Guardar a data, guardo cada valor em cada vari�vel
	int porFora; //Vari�vel pra dizer se foi deposito anonimo ou n�o
	long numeroConta;

} TUltimosDepositos;

typedef struct STransferencias {

	float valor;
	int dia,mes,ano,hora,minuto,segundo; //Para Guardar a data, guardo cada valor em cada vari�vel
	char nomePessoaRecebeu[70];
	long numeroConta;

} TUltimasTransferencias;

typedef struct Sconta {

	int agencia;
	int operacao;
	long numeroConta;
	long senha;
	float saldo;

} TConta;

typedef struct SPessoa {
	char nome[70];
	char cpf[15];
	char sexo[12];
	int idade;
	TConta conta;

} TPessoa;

/* Fim Declara��o Structs */

//Declara��o de v�riaveis globais

FILE *file;
FILE *fileExtrato;
TPessoa pessoaLogada;
char senhaAdmin[6] = "admin";

/* Fim Declara��o v�riaveis globais */


/* Declara��o das Fun��es*/
/* Fun��es para gerar menus */

void acessarSistema();
void sair();
void menuDeAcesso(TPessoa pessoa);
void loginSistema();
void adminAcesso();
void menuMais(TPessoa pessoa);

//Fun��o de CRUD

void cadastrarUsuario();
void editarUsuario(TPessoa pessoa);
void verInformacoes(TPessoa pessoa);
void verInformacoesConta(TPessoa pessoa);

//Fun��es Do Caixa

void versaldo(TPessoa pessoa);
void saque(TPessoa pessoa);
void menuExtrato (TPessoa pessoa);
void menuExtratoPeriodo(TPessoa pessoa,int tipoExtrato);
void deposito (TPessoa pessoa);
void depositoAnonimo();
void transferencia(TPessoa pessoa);
void extratoDepositos(TPessoa pessoa);
void extratoTransferencias(TPessoa pessoa);
void extratoSaques(TPessoa pessoa);

//Fun��es para manipular arquivos

/*
	Fun��o Para manipular os dados,
	char modo = caracter pra representar modo de abertura de arquivo
	op��es => 'w' = w+b,'r' = 'rb', 'a' = 'ab', f = 'r+b'
  */
FILE* abreArquivo(char modo);

FILE* abreArquivoDeposito(char modo);

FILE* abreArquivoSaque(char modo);

FILE* abreArquivoTransferencia(char modo);

void fecharArquivo(FILE *arquivo); //Fun��o Para Fechar arquivo

/* Fim Declara��o Fun��es */

//Main
int main() {

	//trecho de c�digo para desabilitar o bot�o fechar do cmd

	HWND hnd;
	HMENU menu;
	int i, j, cont;
	LPTSTR buffer;

	// Recebe o handle do console
	hnd = GetConsoleWindow();

	// Recebe o handle para o menu do sistema
	menu = GetSystemMenu(hnd, 0);

	// Recebe quantos itens tem no menu
	cont = GetMenuItemCount(menu);

	j = -1;

	// Alocando espa�o para receber a string que "n�o sabemos" o tamanho
	buffer = (TCHAR*) malloc (256 *sizeof(TCHAR));

	for (i=0; i<cont; i++) {
		// Recebe a string do menu, na posi��o "i" e a coloca no buffer
		GetMenuString(menu, i, buffer, 255, MF_BYPOSITION);
		//Compara se a string do menu � igual a string "Fechar"
		if (!strcmp(buffer, "&Fechar")) {
			j = i;
			break;
		}
	}
	// Se a string "Fechar" for encontrada, remove do menu
	if (j >= 0)
		RemoveMenu(menu, j, MF_BYPOSITION);

	//trecho de c�digo para desabilitar o bot�o fechar do cmd

	system("title SIMULADOR BANCO");
	system("color 8f");
	setlocale(LC_ALL, "Portuguese");
	acessarSistema();

}
//Fim Main

//Implementa��o de Fun��es

//Fun��o De Acesso Ao Sistema
void acessarSistema() {
	system("color 8f");
	system("cls");
	int opc;
	printf("\n\t\t\tSimulador de Sistema Banc�rio \n");
	printf("\n---------------------------------------------------\n");
	printf("\n\n\t\t\t MENU\n\n");
	printf("\n---------------------------------------------------\n");
	printf("1- Acesso do usu�rio");
	printf("\n2- Cadastrar Novo Usu�rio");
	printf("\n3- Depositar ");
	printf("\n4- Sair ");
	printf("\n\nDigite uma op��o: ");
	fflush(stdin);
	scanf("%i",&opc);
	switch(opc) {
		case 1:
			loginSistema();
			break;
		case 2:
			adminAcesso();
			break;
		case 3:
			depositoAnonimo();
			break;
		case 4:
			sair();
			break;
		default :
			printf("\n Op��o Inv�lida, Digite uma das op��es dispon�veis\n");
			system("pause");
			acessarSistema();
			break;
	}

}

//Fun��o Que Verifica Admin
void adminAcesso(){
	char senha[30];
	int numeroTentativas = 3;
	system("cls");
	system("color 3f");
	
	int j;
	for (j = 0; j < numeroTentativas ; j++) {
		printf("\n\n N�mero de tentativas: %i, Resta:  %i \n\n",(j),numeroTentativas - (j));
		printf("\n\nDigite a senha do admin: ");
		fflush(stdin);
		gets(senha);
		
		if (strlen(senha) == 0 ){
			printf("\n Digite Algo Para A Senha \n");
			system("pause");
			system("cls");
		}else{
			
			if (strcmp(senha,senhaAdmin) == 0){
				cadastrarUsuario();
				break;
			}else {
				printf("\n Senha Inv�lida \n");
				system("pause");
				system("cls");
			}
			
		}
		
		
	}
	
	printf("\nQuantidade de tentativas Esgotadas, Tente Novamente \n");
	system("pause");
	acessarSistema();
	
}

//Fun��o Para Verificar Se Conta Existe
void loginSistema() {
	TPessoa aux;
	long numeroConta;
	int usuarioValido = 0;
	int numeroTentativas = 5;
	system("cls");
	system("color 3f");
	//Pegar Arquivo E Jogar Em Array
	int j;
	for (j = 0; j < numeroTentativas ; j++) {
		printf("\n\n N�mero de tentativas: %i, Resta:  %i \n\n",(j),numeroTentativas - (j));
		printf("\n\nDigite N�mero Da Conta(s� n�meros): ");
		fflush(stdin);
		scanf("%d",&numeroConta);

		file = abreArquivo('r');
		if (numeroConta != 0) {



			while( fread(&aux,sizeof(TPessoa),1,file) == 1) {

				if (aux.conta.numeroConta == numeroConta) {
					pessoaLogada = aux;
					usuarioValido = 1;
				}

			}

			if (usuarioValido == 1) {
				fecharArquivo(file);
				menuDeAcesso(pessoaLogada);
				break;

			} else {

				printf("\n Usu�rio N�o Existe No Sistema\n");
				system("pause");
				system("cls");
			}

		} else {
			printf("\n Digite S� N�meros!\n");
			system("pause");
			system("cls");
		}

	}

	fecharArquivo(file);

	printf("\nQuantidade de tentativas Esgotadas, Tente Novamente Ou Fa�a O Cadastro \n");
	system("pause");
	acessarSistema();


}


//Fun��o de Menu Com Op��es do sistema
void menuDeAcesso(TPessoa pessoa) {
	int opc;
	system("cls");
	system("color 3f");
	printf("\n\t\t\tSimulador De Sistema Banc�rio\n");
	printf("\n---------------------------------------------------\n");

	printf("\n Bem Vindo %s \n",pessoa.nome);

	printf("\n---------------------------------------------------\n");

	printf("\n\n\t\t\t MENU\n\n");
	printf("1- Ver Saldo");
	printf("\n2- Extrato");
	printf("\n3- Saque");
	printf("\n4- Dep�sito");
	printf("\n5- Transfer�ncia");
	printf("\n6- Mais");
	printf("\n7- Menu Principal ");
	printf("\n8- Sair Do Sistema\n");
	printf("\n\nDigite uma op��o: ");
	fflush(stdin);
	scanf("%i",&opc);
	switch(opc) {
		case 1:
			versaldo(pessoa);
			break;
		case 2:
			menuExtrato(pessoa);
			break;
		case 3:
			saque(pessoa);
			break;
		case 4:
			deposito(pessoa);
			break;
		case 5:
			transferencia(pessoa);
			break;
		case 6:
			menuMais(pessoa);
			break;

		case 7:
			acessarSistema();

			break;

		case 8:
			sair();
			break;

		default:
			printf("Op��o invalida!\n");
			system("pause");
			menuDeAcesso(pessoa);
			break;
	}

}

void menuMais(TPessoa pessoa) {
	system("color 3f");
	system("cls");
	int opc;
	printf("\n---------------------------------------------------\n\n");
	printf("1- Ver Informa��es Do Usu�rio");
	printf("\n2- Ver informa��es da conta");
	printf("\n3- Editar Informa��es Do Usu�rio");
	printf("\n4- Voltar ");
	printf("\n\nDigite uma op��o: ");
	fflush(stdin);
	scanf("%i",&opc);
	switch(opc) {
		case 1:
			verInformacoes(pessoa);
			break;
		case 2:
			verInformacoesConta(pessoa);
			break;
		case 3:
			editarUsuario(pessoa);
			break;
		case 4:
			menuDeAcesso(pessoa);
			break;
		default :
			printf("\n Op��o Inv�lida, Digite uma das op��es dispon�veis\n");
			system("pause");
			menuMais(pessoa);
			break;
	}

}//Fim Metodo

void sair() {

	system("cls");
	printf("\n\t\n Saindo Do Sistema\n");
	printf("\n\t At� Breve \n\n");
	exit(0);
	system("pause");

}//Fim Metodo



void cadastrarUsuario() {

	TPessoa pessoa;
	TPessoa aux;
	int escolhaSexo;
	int numerocontaValidar = 0;
	int achouConta = 0;
	int achouCpf = 0;
	int validaCpf = 0;

	system("cls");


	do {
		printf("\n\n Digite O Nome Do Usu�rio : ");
		fflush(stdin);
		gets(pessoa.nome);

		if (strlen(pessoa.nome) != 0) {


		} else {
			printf("\n Digite Algo Para Nome \n");
			system("pause");
			system("cls");
		}

	} while (strlen(pessoa.nome) == 0);


	do {

		printf("\n\n Digite o Cpf(###.###.###-##): ");
		fflush(stdin);
		gets(pessoa.cpf);

		if (strlen(pessoa.cpf) != 0) {

			if (strlen(pessoa.cpf) == 14) {

				file = fopen("dados.txt","rb");

				if (file != NULL) {

					while( fread(&aux,sizeof(TPessoa),1,file) == 1) {

						if (strcmp(pessoa.cpf, aux.cpf) == 0) {
							achouCpf = 1;
							break;
						} else {
							achouCpf = 0;
						}

					}

					if (achouCpf == 0 ) {
						validaCpf = 1;
					}


				} else {
					validaCpf = 1;
				}

				fclose(file);

				if (validaCpf == 0) {
					printf("\n Esse CPF j� existe \n");
					system("pause");
					system("cls");
				}

			} else {
				printf("\n Digite apenas 14 digitos no formato (###.###.###-##) \n");
				system("pause");
				system("cls");
			}

		} else {
			printf("\n Digite algo Para O CPF !! \n");
			system("pause");
			system("cls");



		}

	} while((strlen(pessoa.cpf) < 14) || (validaCpf != 1));

	do {

		printf("\n\n Escolha O Sexo : 1- masculino / 2- feminino : ");
		fflush(stdin);
		scanf("%i",&escolhaSexo);
		switch(escolhaSexo) {

			case  1:
				strcpy(pessoa.sexo,"masculino");
				break;
			case  2:
				strcpy(pessoa.sexo,"feminino");
				break;
			default :
				printf("\nOp��o Inv�lida\n");
				system("pause");
				system("cls");
				break;

		}

	} while (escolhaSexo != 1 && escolhaSexo != 2);

	do {
		printf("\n Digite sua idade : ");
		fflush(stdin);
		scanf("%d",&pessoa.idade);

		if (pessoa.idade < 16) {
			printf("\n Sua Idade tem que ser maior que ou igual a 16\n");
			system("pause");
			system("cls");
		}

	} while (pessoa.idade < 16);

	do {
		printf("\n\n Digite A Agencia : ");
		fflush(stdin);
		scanf("%d", &pessoa.conta.agencia);

		if (pessoa.conta.agencia <= 0) {
			printf("\n Digite um valor v�lido, n�mero ou valor maior que zero\n");
			system("pause");
			system("cls");
		}

	} while (pessoa.conta.agencia <= 0);



	do {
		printf("\n\n Digite a opera��o : ");
		fflush(stdin);
		scanf("%d", &pessoa.conta.operacao);

		if (pessoa.conta.operacao <= 0) {
			printf("\n Digite um valor v�lido, n�mero ou valor maior que zero\n");
			system("pause");
			system("cls");
		}

	} while (pessoa.conta.operacao <= 0);

	do {
		printf("\n\n Digite o n�mero da conta : ");
		fflush(stdin);
		scanf("%d",&pessoa.conta.numeroConta);

		if (pessoa.conta.numeroConta <= 0) {
			printf("\n Digite um valor maior que zero \n");
			system("pause");
			system("cls");

		} else {

			file = fopen("dados.txt","rb");

			if (file != NULL) {

				while( fread(&aux,sizeof(TPessoa),1,file) == 1) {

					if (aux.conta.numeroConta == pessoa.conta.numeroConta) {
						printf("\n Esse n�mero de conta j� existe \n");
						system("pause");
						system("cls");
						achouConta = 1;
						break;
					} else {
						achouConta = 0;
					}

				}

				if (achouConta == 0 ) {
					printf("\n");
					numerocontaValidar = 1;
				}


			} else {
				numerocontaValidar = 1;
			}

			fclose(file);

		}


	} while(numerocontaValidar == 0);


	do {

		printf("\n\n Digite A Senha(s� n�meros) : ");
		fflush(stdin);
		scanf("%d", &pessoa.conta.senha);

		if (pessoa.conta.senha <= 0) {
			printf("\n Digite um valor v�lido, n�mero ou valor maior que zero\n");
			system("pause");
			system("cls");
		}

	} while(pessoa.conta.senha <= 0);


	pessoa.conta.saldo = 100.0;

	printf("\nUsu�rio Cadastrado com sucesso, Voc� j� pode acessar o sistema\n");

	file = abreArquivo('a');

	fwrite(&pessoa,sizeof(TPessoa),1,file);

	fecharArquivo(file);


	system("pause");
	acessarSistema();

} //Fim Metodo

void editarUsuario(TPessoa pessoa) {
	TPessoa aux;
	int opcao;
	long senha;
	char cpfAux[15];
	char novoNome[70];
	char novoCpf[15];
	char novoSexo[12];
	int novaIdade;
	long novaSenha = 0;
	int posicao = 0;
	int pegou = 0;
	int escolhaSexo = 0;
	int achouCpf = 0;
	int validaCpf = 0;


	system("cls");
	printf("\n\n\t\t\t Opc�es De Edi��o \n");
	printf("\n1- Nome ");
	printf("\n2- CPF ");
	printf("\n3- Sexo  ");
	printf("\n4- Idade ");
	printf("\n5- Senha ");
	printf("\n6- Voltar ");


	printf("\n\n Digite Uma Op��o: ");
	fflush(stdin);

	scanf("%i", &opcao);

	switch(opcao) {

		case 1:

			system("cls");
			do {
				printf("\nDigite Um Novo Nome : ");
				fflush(stdin);
				gets(novoNome);

				if (strlen(novoNome) <= 0) {
					printf("\n Digite algo para o nome \n");
					system("pause");
					system("cls");
				} else {



					printf("\n Digite A Senha Para Confirmar : ");
					fflush(stdin);
					scanf("%i",&senha);


					if (pessoa.conta.senha == senha) {

						file = abreArquivo('f');


						fseek(file,posicao,SEEK_SET);

						while( fread(&aux,sizeof(TPessoa),1,file) == 1) {

							if (aux.conta.numeroConta == pessoa.conta.numeroConta) {

								strcpy(aux.nome,novoNome);

								strcpy(pessoa.nome,novoNome);

								fseek(file,posicao, SEEK_SET);
								fwrite(&aux,sizeof(TPessoa),1,file);

								pegou = 1;
								break;
							}

							posicao = posicao + sizeof(TPessoa);

						}

						fecharArquivo(file);

						if (pegou == 1) {
							printf("\n Nome atualizado com sucesso \n");
							system("pause");
							editarUsuario(pessoa);
							break;
						} else {
							printf("\n Erro ao tentar salvar nome, tente novamente !!! \n");
							system("pause");
							editarUsuario(pessoa);
							break;
						}

					} else {
						printf("\n Senha Incorreta, Tente Novamente \n");
						system("pause");
						editarUsuario(pessoa);
						break;

					}

				}

			} while(strlen(novoNome) <= 0);


			break;

		case 2:

			system("cls");
			do {
				printf("\n Digite Um Novo CPF(###.###.###-##) : ");
				fflush(stdin);
				gets(novoCpf);

				if (strlen(novoCpf) <= 0) {
					printf("\n Digite algo para o CPF \n");
					system("pause");
					system("cls");
				} else if (strlen(novoCpf) < 14) {
					printf("\n Digite 14 digitos para o CPF (###.###.###-##) \n");
					system("pause");
					system("cls");
				} else if (strlen(novoCpf) > 14) {
					printf("\n Digite 14 digitos para o CPF (###.###.###-##) \n");
					system("pause");
					system("cls");
				} else {

					file = fopen("dados.txt","rb");

					if (file != NULL) {

						while( fread(&aux,sizeof(TPessoa),1,file) == 1) {

							if (strcmp(pessoa.cpf, aux.cpf) == 0) {
								achouCpf = 1;
								break;
							} else {
								achouCpf = 0;
							}

						}

						if (achouCpf == 0 ) {
							validaCpf = 1;
						}


					} else {
						validaCpf = 1;
					}

					fclose(file);

					if (validaCpf == 0) {
						printf("\n Esse CPF j� existe \n");
						system("pause");
						editarUsuario(pessoa);
					} else {


						printf("\n Digite A Senha Para Confirmar : ");
						fflush(stdin);
						scanf("%i",&senha);


						if (pessoa.conta.senha == senha) {

							file = abreArquivo('f');


							fseek(file,posicao,SEEK_SET);

							while( fread(&aux,sizeof(TPessoa),1,file) == 1) {

								if (aux.conta.numeroConta == pessoa.conta.numeroConta) {

									strcpy(aux.cpf,novoCpf);

									strcpy(pessoa.cpf,novoCpf);

									fseek(file,posicao, SEEK_SET);
									fwrite(&aux,sizeof(TPessoa),1,file);

									pegou = 1;
									break;
								}

								posicao = posicao + sizeof(TPessoa);

							}

							fecharArquivo(file);

							if (pegou == 1) {
								printf("\n CPF atualizado com sucesso \n");
								system("pause");
								editarUsuario(pessoa);
								break;
							} else {
								printf("\n Erro ao tentar salvar CPF, tente novamente !!! \n");
								system("pause");
								editarUsuario(pessoa);
								break;
							}

						} else {
							printf("\n Senha Incorreta, Tente Novamente \n");
							system("pause");
							editarUsuario(pessoa);
							break;

						}

					}

				}

			} while(strlen(novoCpf) != 14);

			break;

		case 3:

			system("cls");

			do {

				printf("\n\n Escolha O Sexo : 1- masculino / 2- feminino : ");
				fflush(stdin);
				scanf("%i",&escolhaSexo);
				switch(escolhaSexo) {

					case  1:
						strcpy(novoSexo,"masculino");
						break;
					case  2:
						strcpy(novoSexo,"feminino");
						break;
					default :
						printf("\nOp��o Inv�lida\n");
						system("pause");
						system("cls");
						break;

				}

			} while (escolhaSexo != 1 && escolhaSexo != 2);

			system("cls");
			printf("\n Tem certeza dessa operac�o ? 1- sim / 2- n�o ");
			fflush(stdin);
			scanf("%i",&pegou);

			if (pegou == 1) {

				file = abreArquivo('f');


				fseek(file,posicao,SEEK_SET);

				while( fread(&aux,sizeof(TPessoa),1,file) == 1) {

					if (aux.conta.numeroConta == pessoa.conta.numeroConta) {

						strcpy(aux.sexo,novoSexo);

						strcpy(pessoa.sexo,novoSexo);

						fseek(file,posicao, SEEK_SET);
						fwrite(&aux,sizeof(TPessoa),1,file);

						pegou = 1;
						break;
					}

					posicao = posicao + sizeof(TPessoa);

				}

				fecharArquivo(file);

				printf("\n Sexo atualizado com sucesso \n");
				system("pause");
				editarUsuario(pessoa);


			} else if (pegou = 2) {

				printf("\n Opera��o cancelada \n");
				system("pause");
				editarUsuario(pessoa);

			} else {

				printf("\n Op��o inv�lida \n");
				system("pause");
				editarUsuario(pessoa);

			}

			break;

		case 4:

			system("cls");

			do {

				printf("Digite a nova idade : ");
				fflush(stdin);
				scanf("%d",&novaIdade);

				if (novaIdade < 16) {
					printf("\n Idade tem que ser maior do que 16 anos \n");
					system("pause");
					system("cls");

				}

			} while (novaIdade < 16);



			printf("\n Digite A Senha Para Confirmar : ");
			fflush(stdin);
			scanf("%i",&senha);

			if (pessoa.conta.senha == senha) {

				file = abreArquivo('f');


				fseek(file,posicao,SEEK_SET);

				while( fread(&aux,sizeof(TPessoa),1,file) == 1) {

					if (aux.conta.numeroConta == pessoa.conta.numeroConta) {

						aux.idade = novaIdade;

						pessoa.idade = novaIdade;

						fseek(file,posicao, SEEK_SET);
						fwrite(&aux,sizeof(TPessoa),1,file);

						pegou = 1;
						break;
					}

					posicao = posicao + sizeof(TPessoa);

				}

				fecharArquivo(file);

				if (pegou == 1) {
					printf("\n Idade atualizada com sucesso \n");
					system("pause");
					editarUsuario(pessoa);
				} else {
					printf("\n Erro ao tentar atualizar idade, tente novamente !!! \n");
					system("pause");
					editarUsuario(pessoa);

				}

			} else {
				printf("\n Senha Incorreta, Tente Novamente \n");
				system("pause");
				editarUsuario(pessoa);

			}

			break;


		case 5:

			system("cls");

			do {
				printf("\nDigite Sua Senha Nova : ");
				fflush(stdin);
				scanf("%d", &novaSenha);

				if (novaSenha <= 0) {
					printf("\n Digite um valor v�lido, n�mero ou valor maior que zero\n");
					system("pause");
					system("cls");
				}

			} while(novaSenha <= 0);

			do {

				printf("Digite Seu CPF para Confimar(###.###.###-##) : ");
				fflush(stdin);
				gets(cpfAux);

				if (strlen(cpfAux) < 14 || strlen(cpfAux) > 14) {
					printf("\n Digite 14 digitos para o CPF(###.###.###-##) \n");
					system("pause");
					system("cls");
				}

			} while (strlen(cpfAux) != 14);


			if (strcmp(pessoa.cpf, cpfAux) == 0 ) {

				file = abreArquivo('f');


				fseek(file,posicao,SEEK_SET);

				while( fread(&aux,sizeof(TPessoa),1,file) == 1) {

					if (aux.conta.numeroConta == pessoa.conta.numeroConta) {

						aux.conta.senha  = novaSenha;

						pessoa.conta.senha = novaSenha;

						fseek(file,posicao, SEEK_SET);
						fwrite(&aux,sizeof(TPessoa),1,file);

						pegou = 1;
						break;
					}

					posicao = posicao + sizeof(TPessoa);

				}

				fecharArquivo(file);

				if (pegou == 1) {
					printf("\n Senha atualizada com sucesso \n");
					system("pause");
					editarUsuario(pessoa);
				} else {
					printf("\n Erro ao tentar atualizar senha, tente novamente !!! \n");
					system("pause");
					editarUsuario(pessoa);

				}


			} else {

				printf("\nCpf Incorreto , Tente Novamente !!!\n");
				system("pause");
				editarUsuario(pessoa);

			}

			break;

		case 6:
			system("cls");
			menuMais(pessoa);

		default :
			printf("Opc�o Inv�lida \n");
			system("pause");
			editarUsuario(pessoa);

	}



}//Fim Metodo

void verInformacoes(TPessoa pessoa) {

	system("cls");

	printf("\n Informa��es Do Usu�rio \n");
	printf("\n---------------------------------------------------\n\n");
	printf ("\t\t Nome : %s", pessoa.nome);
	printf("\n\t\t CPF : %s", pessoa.cpf);
	printf("\n\t\t Sexo : %s", pessoa.sexo);
	printf("\n\t\t Idade : %d Anos", pessoa.idade);
	printf("\n---------------------------------------------------\n\n");
	system("pause");

	menuMais(pessoa);

}//Fim Metodo

void versaldo(TPessoa pessoa) {
	TPessoa aux;
	long senha;

	system("cls");
	printf("\nDigite Sua Senha : ");
	fflush(stdin);
	scanf("%i",&senha);

	if (pessoa.conta.senha == senha) {

		file = abreArquivo('r');

		while( fread(&aux,sizeof(TPessoa),1,file) == 1) {

			if (aux.conta.senha == pessoa.conta.senha) {
				printf("\n Seu Saldo � :  R$ %.2f\n",aux.conta.saldo);
				system("Pause");
				menuDeAcesso(pessoa);
				fecharArquivo(file);
			}

		}


	} else {

		printf("\nSenha Incorreta !!\n");
		system("pause");
		menuDeAcesso(pessoa);

	}

}//Fim Metodo


void saque(TPessoa pessoa) {
	TUltimosSaques auxSaque;
	TUltimosSaques ultimosSaque;
	TPessoa aux;
	int pegou = 0;
	int posicao = 0;
	float valor;
	long senha;
	long numContaAux;

	system("cls");
	printf("\nDigite O Valor A Ser Sacado : ");
	fflush(stdin);
	scanf("%f",&valor);

	if (valor <= 0) {

		printf("\n O Valor Tem Que Ser Maior Que Zero \n");
		system("pause");
		saque(pessoa);

	} else {

		printf("\nDigite Sua Senha : ");
		fflush(stdin);
		scanf("%i",&senha);

		if (pessoa.conta.senha == senha) {

			file = abreArquivo('f');

			fseek(file,posicao,SEEK_SET);

			while( fread(&aux,sizeof(TPessoa),1,file) == 1) {

				if (aux.conta.numeroConta == pessoa.conta.numeroConta) {
					if (aux.conta.saldo < valor) {
						printf("\n Saldo insuficiente para realizar o saque\n");
						system("pause");
						menuDeAcesso(pessoa);
						fecharArquivo(file);
						break;
					} else {

						numContaAux = aux.conta.numeroConta;

						aux.conta.saldo = aux.conta.saldo - valor;

						pessoa.conta.saldo = pessoa.conta.saldo - valor;

						fseek(file,posicao,SEEK_SET);

						fwrite(&aux,sizeof(TPessoa),1,file);

					}

					pegou = 1;
					break;
				}
				posicao = posicao + sizeof(TPessoa);
			}

			fecharArquivo(file);

			if (pegou == 1) {

				struct tm *local;
				time_t t;

				t= time(NULL);//Iniciando A V�riavel
				local=localtime(&t);

				ultimosSaque.valor = valor;
				ultimosSaque.dia = local->tm_mday;
				ultimosSaque.mes = local->tm_mon + 1;
				ultimosSaque.ano = local->tm_year + 1900;
				ultimosSaque.hora = local->tm_hour;
				ultimosSaque.minuto = local->tm_min;
				ultimosSaque.segundo = local->tm_sec;
				ultimosSaque.numeroConta = numContaAux;

				fileExtrato = abreArquivoSaque('a');

				if (fileExtrato == NULL) {

					printf("\n N�o foi possivel abrir o arquivo \n");
					system("pause");
					menuDeAcesso(pessoa);

				} else {

					fwrite(&ultimosSaque,sizeof(TUltimosSaques),1,fileExtrato);

				}
				fecharArquivo(fileExtrato);

				printf("\n Saque Realizado Com Sucesso \n");
				printf("\n Seu novo Saldo � : %.2f \n", pessoa.conta.saldo);
				system("pause");
				menuDeAcesso(pessoa);

			} else {

				printf("\n Conta N�o T� Cadastrada No Sistema \n");
				system("pause");
				menuDeAcesso(pessoa);

			}

		} else {

			printf("\nSenha Incorreta !!\n");
			system("pause");
			menuDeAcesso(pessoa);

		}

	}
}//Fim Metodo


void menuExtrato (TPessoa pessoa) {
	int opcao;

	system("cls");

	printf("\n\n 1 - �ltimos Depositos ");
	printf ("\n 2 - �ltimos Saques ");
	printf("\n 3 - �ltimas Transfer�ncias ");
	printf("\n 4 - Voltar");
	printf("\n Digite sua op��o : ");
	fflush(stdin);
	scanf("%i",&opcao);

	switch(opcao) {

		case 1:
			extratoDepositos(pessoa);
			break;
		case 2:
			extratoSaques(pessoa);
			break;
		case 3:
			extratoTransferencias(pessoa);
			break;
		case 4:
			menuDeAcesso(pessoa);
			break;
		default:
			printf("\n Op��o Inv�lida \n");
			system("pause");
			menuExtrato(pessoa);
	}


}//Fim Metodo

void deposito(TPessoa pessoa) {
	TUltimosDepositos auxDeposito;//Para reposicionar array quando tiver cheio
	TUltimosDepositos ultimoDeposito;
	TPessoa aux;
	long numContaAux = 0;
	float valor = 0;
	float valorAux = 0;
	int pegou = 0;
	int posicao = 0;

	system("cls");

	printf("\n Digite O Valor A Ser Depositado : ");
	fflush(stdin);
	scanf("%f", &valor);

	if (valor <= 0) {

		printf("\n O Valor Tem Que Ser Maior que zero \n");
		system("pause");
		deposito(pessoa);

	} else {

		file = abreArquivo('f');

		fseek(file,posicao,SEEK_SET);

		while( fread(&aux,sizeof(TPessoa),1,file) == 1) {

			if (aux.conta.numeroConta == pessoa.conta.numeroConta) {
				valorAux = aux.conta.saldo + valor;
				numContaAux = aux.conta.numeroConta;
				aux.conta.saldo = valorAux;

				pessoa.conta.saldo = valorAux;

				fseek(file,posicao, SEEK_SET);

				fwrite(&aux,sizeof(TPessoa),1,file);

				pegou = 1;
				break;
			}

			posicao = posicao + sizeof(TPessoa);

		}

		fecharArquivo(file);


		if (pegou == 1) {

			struct tm *local;
			time_t t;

			t= time(NULL);//Iniciando A V�riavel
			local=localtime(&t);

			ultimoDeposito.valor = valor;
			ultimoDeposito.dia = local->tm_mday;
			ultimoDeposito.mes = local->tm_mon + 1;
			ultimoDeposito.ano = local->tm_year + 1900;
			ultimoDeposito.hora = local->tm_hour;
			ultimoDeposito.minuto = local->tm_min;
			ultimoDeposito.segundo = local->tm_sec;
			ultimoDeposito.porFora = 0;
			ultimoDeposito.numeroConta = numContaAux;

			fileExtrato = abreArquivoDeposito('a');


			if (fileExtrato == NULL) {
				printf("\n N�o foi possivel abrir o arquivo \n ");
				system("pause");
				menuDeAcesso(pessoa);
			} else {

				fwrite(&ultimoDeposito,sizeof(TUltimosDepositos),1,fileExtrato);

			}

			fclose(fileExtrato);


			printf("\nDeposito Realizado Com Sucesso \n");
			printf("\n Seu novo Saldo � : %.2f \n", pessoa.conta.saldo);
			system("pause");
			menuDeAcesso(pessoa);

		} else {

			printf("\n Conta N�o T� Cadastrada No Sistema \n");
			system("pause");
			menuDeAcesso(pessoa);

		}

	}

}//Fim Metodo

void depositoAnonimo() {

	TUltimosDepositos ultimoDeposito;
	TPessoa aux;
	long numConta;
	float valor;
	float valorAux = 0;
	int pegou = 0;
	int posicao = 0;
	long numContaAux;

	system("cls");

	printf("\n Digite O Valor A Ser Depositado : ");
	fflush(stdin);
	scanf("%f", &valor);

	if (valor <= 0) {
		printf("\n O Valor Tem Que Ser Maior que zero \n");
		system("pause");
		depositoAnonimo();

	} else {

		printf("\n Digite O N�mero Da Conta A Ser Depositada: ");
		fflush(stdin);
		scanf("%i", &numConta);

		file = abreArquivo('f');


		while( fread(&aux,sizeof(TPessoa),1,file) == 1) {

			if (aux.conta.numeroConta == numConta) {
				valorAux = aux.conta.saldo + valor;
				numContaAux = aux.conta.numeroConta;
				aux.conta.saldo = valorAux;


				fseek(file,posicao, SEEK_SET);
				fwrite(&aux,sizeof(TPessoa),1,file);

				pegou = 1;
				break;
			}

			posicao = posicao + sizeof(TPessoa);

		}

		fecharArquivo(file);


		if (pegou == 1) {

			struct tm *local;
			time_t t;

			t= time(NULL);//Iniciando A V�riavel
			local=localtime(&t);

			ultimoDeposito.valor = valor;
			ultimoDeposito.dia = local->tm_mday;
			ultimoDeposito.mes = local->tm_mon + 1;
			ultimoDeposito.ano = local->tm_year + 1900;
			ultimoDeposito.hora = local->tm_hour;
			ultimoDeposito.minuto = local->tm_min;
			ultimoDeposito.segundo = local->tm_sec;
			ultimoDeposito.porFora = 1;
			ultimoDeposito.numeroConta = numContaAux;

			fileExtrato = abreArquivoDeposito('a');
			if (fileExtrato == NULL) {
				printf("\n N�o foi possivel abrir o arquivo \n ");
				system("pause");
				acessarSistema();
			} else {

				fwrite(&ultimoDeposito,sizeof(TUltimosDepositos),1,fileExtrato);

			}

			fclose(fileExtrato);


			printf("\nDeposito Realizado Com Sucesso \n");
			system("pause");
			acessarSistema();

		} else {

			printf("\n Conta N�o T� Cadastrada No Sistema \n");
			system("pause");
			acessarSistema();

		}

	}

}//Fim Metodo

void transferencia(TPessoa pessoa) {
	TUltimasTransferencias ultimaTransferencia;
	char nomeQuemRecebeu[70]; //Para Guardar O Nome de quem recebeu
	TPessoa aux;
	system("cls");
	float valor;
	long numConta;
	long senha;
	int pegou = 0;
	int posicaoPrimeiro = 0; //Primeira pessoa a ter os campos alterados
	int posicaoSegundo = 0; //Segunda Pessoa a ter os campos alterados
	long numContaAux;

	printf("\n Digite O Valor A Ser Transferindo : ");
	fflush(stdin);
	scanf("%f",&valor);

	if (valor <= 0) {
		printf("\n Valor Tem Que Ser Maior Que Zero \n");
		system("pause");
		transferencia(pessoa);
	} else {

		printf("\n Digite A Conta Onde O Valor Ser� Transferido: ");
		fflush(stdin);
		scanf("%i", &numConta);

		if (numConta == pessoa.conta.numeroConta) {
			printf("\n N�o Pode Ser A Mesma Conta \n");
			system("pause");
			menuDeAcesso(pessoa);
		} else {

			printf("\n Digite A Senha: ");
			fflush(stdin);
			scanf("%i", &senha);

			if (pessoa.conta.senha == senha) {
				system("cls");
				if (pessoa.conta.saldo >= valor) {

					file = abreArquivo('f');


					fseek(file,posicaoPrimeiro,SEEK_SET);

					while( fread(&aux,sizeof(TPessoa),1,file) == 1) {

						if (aux.conta.numeroConta == numConta) {


							aux.conta.saldo = aux.conta.saldo + valor;

							strcpy(nomeQuemRecebeu, aux.nome);

							fseek(file,posicaoPrimeiro,SEEK_SET);

							fwrite(&aux,sizeof(TPessoa),1,file);

							pegou = 1;
							break;
						}
						posicaoPrimeiro = posicaoPrimeiro + sizeof(TPessoa);
					}

					fecharArquivo(file);

					if (pegou == 1) { //Atualizando o objeto atual

						file = abreArquivo('f');


						fseek(file,posicaoSegundo,SEEK_SET);

						while( fread(&aux,sizeof(TPessoa),1,file) == 1) {

							if (aux.conta.numeroConta == pessoa.conta.numeroConta) {

								aux.conta.saldo = aux.conta.saldo - valor;

								numContaAux = aux.conta.numeroConta;

								pessoa.conta.saldo = pessoa.conta.saldo - valor;

								fseek(file,posicaoSegundo,SEEK_SET);

								fwrite(&aux,sizeof(TPessoa),1,file);

								break;
							}
							posicaoSegundo = posicaoSegundo + sizeof(TPessoa);
						}

						fecharArquivo(file);

						struct tm *local;
						time_t t;

						t= time(NULL);//Iniciando A V�riavel
						local=localtime(&t);

						ultimaTransferencia.valor = valor;
						strcpy(ultimaTransferencia.nomePessoaRecebeu,nomeQuemRecebeu);
						ultimaTransferencia.dia = local->tm_mday;
						ultimaTransferencia.mes = local->tm_mon + 1;
						ultimaTransferencia.ano = local->tm_year + 1900;
						ultimaTransferencia.hora = local->tm_hour;
						ultimaTransferencia.minuto = local->tm_min;
						ultimaTransferencia.segundo = local->tm_sec;
						ultimaTransferencia.numeroConta = numContaAux;

						fileExtrato = abreArquivoTransferencia('a');

						if (fileExtrato == NULL) {
							printf("\n N�o Foi Poss�vel abrir o arquivo \n");
							system("pause");
							menuDeAcesso(pessoa);
						} else {


							fwrite(&ultimaTransferencia,sizeof(TUltimasTransferencias),1,fileExtrato);

						}

						fclose(fileExtrato);

						printf ("\n Transfer�ncia realizada com sucesso !! \n");
						printf ("Seu Saldo Atual � : %.2f\n", pessoa.conta.saldo);
						system("pause");
						menuDeAcesso(pessoa);

					} else {
						printf("\n Conta n�o est� cadastrada no sistema !!! \n");
						system("pause");
						menuDeAcesso(pessoa);
					}

				} else {

					printf("\n Saldo Insuficiente \n");
					printf("Seu Saldo � %.2f \n",pessoa.conta.saldo);
					system("pause");
					menuDeAcesso(pessoa);

				}

			} else {
				printf("\n Senha Incorreta!! \n");
				system("pause");
				menuDeAcesso(pessoa);
			}


		}

	}


}//Fim Metodo


void extratoDepositos(TPessoa pessoa) {
	TUltimosDepositos ultimoDeposito;
	int opcao;
	int contador = 0;
	long posicao = 0;
	system("cls");

	printf("\n\n 1 - �ltimos 5 Depositos ");
	printf ("\n 2 - �ltimos por periodo ");
	printf("\n 3 - Voltar");
	printf("\n Digite sua op��o : ");
	fflush(stdin);
	scanf("%i",&opcao);

	switch(opcao) {

		case 1:
			system("cls");

			fileExtrato = abreArquivoDeposito('r');



			if (fileExtrato == NULL) {

				printf("\n Voc� n�o realizou Depositos ainda \n");
				system("pause");
				menuExtrato(pessoa);

			} else {

				fseek(fileExtrato,0,SEEK_END);

				posicao = ftell(fileExtrato);

				posicao = posicao - sizeof(TUltimosDepositos);


				printf("\n================== �LTIMOS DEPOSITOS ====================\n");

				while( fseek(fileExtrato,posicao,SEEK_SET) != -1) {

					fread(&ultimoDeposito,sizeof(TUltimosDepositos),1,fileExtrato);

					if (ultimoDeposito.numeroConta == pessoa.conta.numeroConta) {



						if (contador < 5) {

							if (ultimoDeposito.porFora == 1) {

								printf ("\n Valor : %.2f    Data : %d/%d/%d    Hora : %d:%d:%d Foi Por Fora",ultimoDeposito.valor,ultimoDeposito.dia,ultimoDeposito.mes,ultimoDeposito.ano,ultimoDeposito.hora,ultimoDeposito.minuto,ultimoDeposito.segundo);

							} else {
								printf ("\n Valor : %.2f    Data : %d/%d/%d    Hora : %d:%d:%d Foi Voc�",ultimoDeposito.valor,ultimoDeposito.dia,ultimoDeposito.mes,ultimoDeposito.ano,ultimoDeposito.hora,ultimoDeposito.minuto,ultimoDeposito.segundo);

							}
							contador++;



						} else {
							break;
						}




					}


					posicao = posicao - sizeof(TUltimosDepositos);
				}


				printf("\n\n========================================================\n");
			}

			fecharArquivo(fileExtrato);

			system("pause");
			menuExtrato(pessoa);

			break;
		case 2:
			menuExtratoPeriodo(pessoa,1);
			break;
		case 3:
			menuExtrato(pessoa);
			break;
		default:
			printf("\n Op��o Inv�lida \n");
			system("pause");
			extratoDepositos(pessoa);
			break;
	}




}//Fim M�todo

void extratoSaques(TPessoa pessoa) {
	TUltimosSaques ultimoSaque;
	int opcao;
	int contador = 0;
	int posicao = 0;
	system("cls");

	printf("\n\n 1 - �ltimos 5 Saques ");
	printf ("\n 2 - �ltimos por periodo ");
	printf("\n 3 - Voltar");
	printf("\n Digite sua op��o : ");
	fflush(stdin);
	scanf("%i",&opcao);

	switch(opcao) {

		case 1:
			system("cls");

			fileExtrato = abreArquivoSaque('r');



			if (fileExtrato == NULL) {

				printf("\n Voc� n�o realizou Saques ainda \n");
				system("pause");
				menuExtrato(pessoa);

			} else {


				fseek(fileExtrato,0,SEEK_END);

				posicao = ftell(fileExtrato);

				posicao = posicao - sizeof(TUltimosSaques);



				printf("\n================== �LTIMOS SAQUES ====================\n");

				while( fseek(fileExtrato,posicao,SEEK_SET) != -1) {

					fread(&ultimoSaque,sizeof(TUltimosSaques),1,fileExtrato);

					if (ultimoSaque.numeroConta == pessoa.conta.numeroConta) {


						if (contador < 5) {
							fseek(fileExtrato,posicao,SEEK_END);

							printf ("\n Valor : %.2f    Data : %d/%d/%d    Hora : %d:%d:%d",ultimoSaque.valor,ultimoSaque.dia,ultimoSaque.mes,ultimoSaque.ano,ultimoSaque.hora,ultimoSaque.minuto,ultimoSaque.segundo);
							contador++;

						} else {
							break;
						}

					}


					posicao = posicao - sizeof(TUltimosSaques);
				}

				printf("\n\n========================================================\n");
			}

			fecharArquivo(fileExtrato);


			system("pause");
			menuExtrato(pessoa);


			break;
		case 2:
			menuExtratoPeriodo(pessoa,2);
			break;
		case 3:
			menuExtrato(pessoa);
			break;
		default:
			printf("\n Op��o Inv�lida \n");
			system("pause");
			extratoDepositos(pessoa);
			break;

	}



}//Fim M�todo

void extratoTransferencias(TPessoa pessoa) {

	TUltimasTransferencias ultimasTransferencias;
	int opcao;
	int posicao = 0;
	int contador = 0;
	system("cls");

	printf("\n\n 1 - �ltimas 5 Transfer�ncias ");
	printf ("\n 2 - �ltimas por periodo ");
	printf("\n 3 - Voltar");
	printf("\n Digite sua op��o : ");
	fflush(stdin);
	scanf("%i",&opcao);

	switch(opcao) {

		case 1:
			system("cls");

			fileExtrato = abreArquivoTransferencia('r');



			if (fileExtrato == NULL) {

				printf("\n Voc� n�o realizou Transfer�ncias ainda \n");
				system("pause");
				menuExtrato(pessoa);

			} else {
				fseek(fileExtrato,0,SEEK_END);

				posicao = ftell(fileExtrato);

				posicao = posicao - sizeof(TUltimasTransferencias);

				printf("\n================== �LTIMAS TRANSFER�NCIAS ====================\n");

				while(fseek(fileExtrato,posicao,SEEK_SET) != -1 ) {

					fread(&ultimasTransferencias,sizeof(TUltimasTransferencias),1,fileExtrato);

					if (ultimasTransferencias.numeroConta == pessoa.conta.numeroConta) {

						if (contador < 5) {

							printf ("\n Valor : %.2f Para : %s   Data : %d/%d/%d    Hora : %d:%d:%d",ultimasTransferencias.valor,ultimasTransferencias.nomePessoaRecebeu,ultimasTransferencias.dia,ultimasTransferencias.mes,ultimasTransferencias.ano,ultimasTransferencias.hora,ultimasTransferencias.minuto,ultimasTransferencias.segundo);

							contador ++;

						} else {
							break;
						}



					}

					posicao = posicao - sizeof(TUltimasTransferencias);
				}

				printf("\n\n========================================================\n");
			}

			fecharArquivo(fileExtrato);

			system("pause");
			menuExtrato(pessoa);

			break;

		case 2:
			menuExtratoPeriodo(pessoa,3);
			break;
		case 3:
			menuExtrato(pessoa);
			break;
		default:
			printf("\n Op��o Inv�lida \n");
			system("pause");
			extratoDepositos(pessoa);
			break;

	}


}//Fim M�todo

void menuExtratoPeriodo(TPessoa pessoa,int tipoExtrato) {
	TUltimosDepositos ultimoDeposito;
	TUltimosSaques ultimoSaque;
	TUltimasTransferencias ultimasTransferencias;
	int opcao;

	system("cls");

	printf("\n\n 1 - �ltimos 5 dias ");
	printf ("\n 2 - �ltimos 15 dias ");
	printf("\n 3 - �ltimos 30 dias");
	printf("\n 4 - voltar ");
	printf("\n Digite sua op��o : ");
	fflush(stdin);
	scanf("%i",&opcao);

	switch(opcao) {

		case 1:

			if (tipoExtrato == 1) {

				int quantidade = 0;
				int posicao = 0;
				int diaAux;
				struct tm *local;
				time_t t;

				t= time(NULL);//Iniciando A V�riavel
				local=localtime(&t);

				int diaAtual = local->tm_mday;
				diaAux = diaAtual;


				system("cls");

				fileExtrato = abreArquivoDeposito('r');

				if (fileExtrato == NULL) {

					printf("\n Voc� n�o realizou Depositos ainda \n");
					system("pause");
					menuExtrato(pessoa);

				} else {

					fseek(fileExtrato,0,SEEK_END);

					posicao = ftell(fileExtrato);

					posicao = posicao - sizeof(TUltimosDepositos);


					printf("\n================== DEPOSITOS �LTIMOS 5 DIAS ====================\n");

					while( fseek(fileExtrato,posicao,SEEK_SET) != -1) {

						fread(&ultimoDeposito,sizeof(TUltimosDepositos),1,fileExtrato);

						if (ultimoDeposito.numeroConta == pessoa.conta.numeroConta) {

							if (quantidade < 5) {

								if (ultimoDeposito.dia == diaAux) {


									if (ultimoDeposito.porFora == 1) {

										printf ("\n Valor : %.2f    Data : %d/%d/%d    Hora : %d:%d:%d Foi Por Fora",ultimoDeposito.valor,ultimoDeposito.dia,ultimoDeposito.mes,ultimoDeposito.ano,ultimoDeposito.hora,ultimoDeposito.minuto,ultimoDeposito.segundo);

									} else {
										printf ("\n Valor : %.2f    Data : %d/%d/%d    Hora : %d:%d:%d Foi Voc�",ultimoDeposito.valor,ultimoDeposito.dia,ultimoDeposito.mes,ultimoDeposito.ano,ultimoDeposito.hora,ultimoDeposito.minuto,ultimoDeposito.segundo);

									}

								} else {
									diaAux--;
									quantidade++;
								}



							} else {
								break;
							}


						}
						posicao = posicao - sizeof(TUltimosDepositos);
					}


					printf("\n\n========================================================\n");
				}

				fecharArquivo(fileExtrato);
				system("pause");
				menuExtrato(pessoa);


			} else if (tipoExtrato == 2) {
				int quantidade = 0;
				int diaAux;
				int posicao = 0;
				struct tm *local;
				time_t t;

				t= time(NULL);//Iniciando A V�riavel
				local=localtime(&t);

				int diaAtual = local->tm_mday;
				diaAux = diaAtual;


				system("cls");

				fileExtrato = abreArquivoSaque('r');

				if (fileExtrato == NULL) {

					printf("\n Voc� n�o realizou Saques ainda \n");
					system("pause");
					menuExtrato(pessoa);

				} else {

					fseek(fileExtrato,0,SEEK_END);

					posicao = ftell(fileExtrato);

					posicao = posicao - sizeof(TUltimosSaques);

					printf("\n================== SAQUES �LTIMOS 5 DIAS ====================\n");

					while( fseek(fileExtrato,posicao,SEEK_SET) != -1) {

						fread(&ultimoSaque,sizeof(TUltimosSaques),1,fileExtrato);

						if (ultimoSaque.numeroConta == pessoa.conta.numeroConta) {

							if (quantidade < 5) {

								if (ultimoSaque.dia == diaAux) {


									printf ("\n Valor : %.2f    Data : %d/%d/%d    Hora : %d:%d:%d",ultimoSaque.valor,ultimoSaque.dia,ultimoSaque.mes,ultimoSaque.ano,ultimoSaque.hora,ultimoSaque.minuto,ultimoSaque.segundo);


								} else {
									diaAux--;
									quantidade++;
								}

							} else {
								break;
							}


						}
						posicao = posicao - sizeof(TUltimosSaques);
					}


					printf("\n\n========================================================\n");
				}

				fecharArquivo(fileExtrato);
				system("pause");
				menuExtrato(pessoa);


			} else if (tipoExtrato == 3) {
				int quantidade = 0;
				int diaAux;
				int posicao = 0;
				struct tm *local;
				time_t t;

				t= time(NULL);//Iniciando A V�riavel
				local=localtime(&t);

				int diaAtual = local->tm_mday;
				diaAux = diaAtual;


				system("cls");

				fileExtrato = abreArquivoTransferencia('r');

				if (fileExtrato == NULL) {

					printf("\n Voc� n�o realizou Transfer�ncias ainda \n");
					system("pause");
					menuExtrato(pessoa);

				} else {
					fseek(fileExtrato,0,SEEK_END);

					posicao = ftell(fileExtrato);

					posicao = posicao - sizeof(TUltimasTransferencias);

					printf("\n================== TRANSFER�NCIAS �LTIMOS 5 DIAS ====================\n");

					while(fseek(fileExtrato,posicao,SEEK_SET) != -1 ) {

						fread(&ultimasTransferencias,sizeof(TUltimasTransferencias),1,fileExtrato);

						if (ultimasTransferencias.numeroConta == pessoa.conta.numeroConta) {

							if (quantidade < 5) {

								if (ultimasTransferencias.dia == diaAux) {


									printf ("\n Valor : %.2f Para : %s   Data : %d/%d/%d    Hora : %d:%d:%d",ultimasTransferencias.valor,ultimasTransferencias.nomePessoaRecebeu,ultimasTransferencias.dia,ultimasTransferencias.mes,ultimasTransferencias.ano,ultimasTransferencias.hora,ultimasTransferencias.minuto,ultimasTransferencias.segundo);


								} else {
									diaAux--;
									quantidade++;
								}

							} else {
								break;
							}


						}

						posicao = posicao - sizeof(TUltimasTransferencias);
					}


					printf("\n\n========================================================\n");
				}

				fecharArquivo(fileExtrato);
				system("pause");
				menuExtrato(pessoa);
			}

			break;

		case 2:

			if (tipoExtrato == 1) {
				int quantidade = 0;
				int posicao = 0;
				int diaAux;
				struct tm *local;
				time_t t;

				t= time(NULL);//Iniciando A V�riavel
				local=localtime(&t);

				int diaAtual = local->tm_mday;
				diaAux = diaAtual;


				system("cls");

				fileExtrato = abreArquivoDeposito('r');

				if (fileExtrato == NULL) {

					printf("\n Voc� n�o realizou Depositos ainda \n");
					system("pause");
					menuExtrato(pessoa);

				} else {

					fseek(fileExtrato,0,SEEK_END);

					posicao = ftell(fileExtrato);

					posicao = posicao - sizeof(TUltimosDepositos);


					printf("\n================== DEPOSITOS �LTIMOS 15 DIAS ====================\n");

					while( fseek(fileExtrato,posicao,SEEK_SET) != -1) {

						fread(&ultimoDeposito,sizeof(TUltimosDepositos),1,fileExtrato);

						if (ultimoDeposito.numeroConta == pessoa.conta.numeroConta) {

							if (quantidade < 15) {

								if (ultimoDeposito.dia == diaAux) {


									if (ultimoDeposito.porFora == 1) {

										printf ("\n Valor : %.2f    Data : %d/%d/%d    Hora : %d:%d:%d Foi Por Fora",ultimoDeposito.valor,ultimoDeposito.dia,ultimoDeposito.mes,ultimoDeposito.ano,ultimoDeposito.hora,ultimoDeposito.minuto,ultimoDeposito.segundo);

									} else {
										printf ("\n Valor : %.2f    Data : %d/%d/%d    Hora : %d:%d:%d Foi Voc�",ultimoDeposito.valor,ultimoDeposito.dia,ultimoDeposito.mes,ultimoDeposito.ano,ultimoDeposito.hora,ultimoDeposito.minuto,ultimoDeposito.segundo);

									}

								} else {
									diaAux--;
									quantidade++;
								}



							} else {
								break;
							}


						}
						posicao = posicao - sizeof(TUltimosDepositos);
					}


					printf("\n\n========================================================\n");
				}

				fecharArquivo(fileExtrato);
				system("pause");
				menuExtrato(pessoa);


			} else if (tipoExtrato == 2) {

				int quantidade = 0;
				int diaAux;
				int posicao = 0;
				struct tm *local;
				time_t t;

				t= time(NULL);//Iniciando A V�riavel
				local=localtime(&t);

				int diaAtual = local->tm_mday;
				diaAux = diaAtual;

				system("cls");

				fileExtrato = abreArquivoSaque('r');

				if (fileExtrato == NULL) {

					printf("\n Voc� n�o realizou Saques ainda \n");
					system("pause");
					menuExtrato(pessoa);

				} else {

					fseek(fileExtrato,0,SEEK_END);

					posicao = ftell(fileExtrato);

					posicao = posicao - sizeof(TUltimosSaques);

					printf("\n================== SAQUES �LTIMOS 15 DIAS ====================\n");

					while( fseek(fileExtrato,posicao,SEEK_SET) != -1) {

						fread(&ultimoSaque,sizeof(TUltimosSaques),1,fileExtrato);

						if (ultimoSaque.numeroConta == pessoa.conta.numeroConta) {

							if (quantidade < 15) {

								if (ultimoSaque.dia == diaAux) {


									printf ("\n Valor : %.2f    Data : %d/%d/%d    Hora : %d:%d:%d",ultimoSaque.valor,ultimoSaque.dia,ultimoSaque.mes,ultimoSaque.ano,ultimoSaque.hora,ultimoSaque.minuto,ultimoSaque.segundo);


								} else {
									diaAux--;
									quantidade++;
								}

							} else {
								break;
							}


						}
						posicao = posicao - sizeof(TUltimosSaques);
					}


					printf("\n\n========================================================\n");
				}

				fecharArquivo(fileExtrato);
				system("pause");
				menuExtrato(pessoa);



			} else if (tipoExtrato == 3) {

				int quantidade = 0;
				int diaAux;
				int posicao = 0;
				struct tm *local;
				time_t t;

				t= time(NULL);//Iniciando A V�riavel
				local=localtime(&t);

				int diaAtual = local->tm_mday;
				diaAux = diaAtual;


				system("cls");

				fileExtrato = abreArquivoTransferencia('r');

				if (fileExtrato == NULL) {

					printf("\n Voc� n�o realizou Transfer�ncias ainda \n");
					system("pause");
					menuExtrato(pessoa);

				} else {
					fseek(fileExtrato,0,SEEK_END);

					posicao = ftell(fileExtrato);

					posicao = posicao - sizeof(TUltimasTransferencias);

					printf("\n================== TRANSFER�NCIAS �LTIMOS 15 DIAS ====================\n");

					while(fseek(fileExtrato,posicao,SEEK_SET) != -1 ) {

						fread(&ultimasTransferencias,sizeof(TUltimasTransferencias),1,fileExtrato);

						if (ultimasTransferencias.numeroConta == pessoa.conta.numeroConta) {

							if (quantidade < 15) {

								if (ultimasTransferencias.dia == diaAux) {


									printf ("\n Valor : %.2f Para : %s   Data : %d/%d/%d    Hora : %d:%d:%d",ultimasTransferencias.valor,ultimasTransferencias.nomePessoaRecebeu,ultimasTransferencias.dia,ultimasTransferencias.mes,ultimasTransferencias.ano,ultimasTransferencias.hora,ultimasTransferencias.minuto,ultimasTransferencias.segundo);


								} else {
									diaAux--;
									quantidade++;
								}

							} else {
								break;
							}


						}

						posicao = posicao - sizeof(TUltimasTransferencias);
					}


					printf("\n\n========================================================\n");
				}

				fecharArquivo(fileExtrato);
				system("pause");
				menuExtrato(pessoa);


			}

			break;

		case 3:

			if (tipoExtrato == 1) {
				int quantidade = 0;
				int posicao = 0;
				int diaAux;
				struct tm *local;
				time_t t;

				t= time(NULL);//Iniciando A V�riavel
				local=localtime(&t);

				int diaAtual = local->tm_mday;
				diaAux = diaAtual;


				system("cls");

				fileExtrato = abreArquivoDeposito('r');

				if (fileExtrato == NULL) {

					printf("\n Voc� n�o realizou Depositos ainda \n");
					system("pause");
					menuExtrato(pessoa);

				} else {

					fseek(fileExtrato,0,SEEK_END);

					posicao = ftell(fileExtrato);

					posicao = posicao - sizeof(TUltimosDepositos);


					printf("\n================== DEPOSITOS �LTIMOS 30 DIAS ====================\n");

					while( fseek(fileExtrato,posicao,SEEK_SET) != -1) {

						fread(&ultimoDeposito,sizeof(TUltimosDepositos),1,fileExtrato);

						if (ultimoDeposito.numeroConta == pessoa.conta.numeroConta) {

							if (quantidade < 30) {

								if (ultimoDeposito.dia == diaAux) {


									if (ultimoDeposito.porFora == 1) {

										printf ("\n Valor : %.2f    Data : %d/%d/%d    Hora : %d:%d:%d Foi Por Fora",ultimoDeposito.valor,ultimoDeposito.dia,ultimoDeposito.mes,ultimoDeposito.ano,ultimoDeposito.hora,ultimoDeposito.minuto,ultimoDeposito.segundo);

									} else {
										printf ("\n Valor : %.2f    Data : %d/%d/%d    Hora : %d:%d:%d Foi Voc�",ultimoDeposito.valor,ultimoDeposito.dia,ultimoDeposito.mes,ultimoDeposito.ano,ultimoDeposito.hora,ultimoDeposito.minuto,ultimoDeposito.segundo);

									}

								} else {
									diaAux--;
									quantidade++;
								}



							} else {
								break;
							}


						}
						posicao = posicao - sizeof(TUltimosDepositos);
					}


					printf("\n\n========================================================\n");
				}

				fecharArquivo(fileExtrato);
				system("pause");
				menuExtrato(pessoa);


			} else if (tipoExtrato == 2) {

				int quantidade = 0;
				int diaAux;
				int posicao = 0;
				struct tm *local;
				time_t t;

				t= time(NULL);//Iniciando A V�riavel
				local=localtime(&t);

				int diaAtual = local->tm_mday;
				diaAux = diaAtual;

				system("cls");

				fileExtrato = abreArquivoSaque('r');

				if (fileExtrato == NULL) {

					printf("\n Voc� n�o realizou Saques ainda \n");
					system("pause");
					menuExtrato(pessoa);

				} else {

					fseek(fileExtrato,0,SEEK_END);

					posicao = ftell(fileExtrato);

					posicao = posicao - sizeof(TUltimosSaques);

					printf("\n================== SAQUES �LTIMOS 30 DIAS ====================\n");

					while( fseek(fileExtrato,posicao,SEEK_SET) != -1) {

						fread(&ultimoSaque,sizeof(TUltimosSaques),1,fileExtrato);

						if (ultimoSaque.numeroConta == pessoa.conta.numeroConta) {

							if (quantidade < 30) {

								if (ultimoSaque.dia == diaAux) {


									printf ("\n Valor : %.2f    Data : %d/%d/%d    Hora : %d:%d:%d",ultimoSaque.valor,ultimoSaque.dia,ultimoSaque.mes,ultimoSaque.ano,ultimoSaque.hora,ultimoSaque.minuto,ultimoSaque.segundo);


								} else {
									diaAux--;
									quantidade++;
								}

							} else {
								break;
							}


						}
						posicao = posicao - sizeof(TUltimosSaques);
					}


					printf("\n\n========================================================\n");
				}

				fecharArquivo(fileExtrato);
				system("pause");
				menuExtrato(pessoa);



			} else if (tipoExtrato == 3) {

				int quantidade = 0;
				int diaAux;
				int posicao = 0;
				struct tm *local;
				time_t t;

				t= time(NULL);//Iniciando A V�riavel
				local=localtime(&t);

				int diaAtual = local->tm_mday;
				diaAux = diaAtual;


				system("cls");

				fileExtrato = abreArquivoTransferencia('r');

				if (fileExtrato == NULL) {

					printf("\n Voc� n�o realizou Transfer�ncias ainda \n");
					system("pause");
					menuExtrato(pessoa);

				} else {
					fseek(fileExtrato,0,SEEK_END);

					posicao = ftell(fileExtrato);

					posicao = posicao - sizeof(TUltimasTransferencias);

					printf("\n================== TRANSFER�NCIAS �LTIMOS 30 DIAS ====================\n");

					while(fseek(fileExtrato,posicao,SEEK_SET) != -1 ) {

						fread(&ultimasTransferencias,sizeof(TUltimasTransferencias),1,fileExtrato);

						if (ultimasTransferencias.numeroConta == pessoa.conta.numeroConta) {

							if (quantidade < 30) {

								if (ultimasTransferencias.dia == diaAux) {


									printf ("\n Valor : %.2f Para : %s   Data : %d/%d/%d    Hora : %d:%d:%d",ultimasTransferencias.valor,ultimasTransferencias.nomePessoaRecebeu,ultimasTransferencias.dia,ultimasTransferencias.mes,ultimasTransferencias.ano,ultimasTransferencias.hora,ultimasTransferencias.minuto,ultimasTransferencias.segundo);


								} else {
									diaAux--;
									quantidade++;
								}

							} else {
								break;
							}


						}

						posicao = posicao - sizeof(TUltimasTransferencias);
					}


					printf("\n\n========================================================\n");
				}

				fecharArquivo(fileExtrato);
				system("pause");
				menuExtrato(pessoa);


			}

			break;

		case 4:
			if (tipoExtrato == 1) {
				extratoDepositos(pessoa);
			} else if (tipoExtrato == 2) {
				extratoSaques(pessoa);
			} else if (tipoExtrato == 3) {
				extratoTransferencias(pessoa);
			}
			break;

		default:
			printf("\n Op��o Inv�lida \n");
			system("pause");
			menuExtratoPeriodo(pessoa,tipoExtrato);
			break;

	}

}

void verInformacoesConta(TPessoa pessoa) {
	system("cls");

	printf("\n==============================");
	printf("\n N�mero da conta : %i",pessoa.conta.numeroConta);
	printf("\n Ag�ncia : %i", pessoa.conta.agencia);
	printf("\n Operador : %i", pessoa.conta.operacao);
	printf("\n================================\n");
	system("pause");
	menuMais(pessoa);

}//Fim M�todo

//Helpers

FILE* abreArquivo(char modo) {
	FILE *arquivo;
	char caminho [30] = "dados.txt";
	switch (modo) {

		case 'w'://Escrever
			arquivo = fopen(caminho,"wb");
			break;
		case 'r'://Ler
			arquivo = fopen(caminho,"rb");
			break;
		case 'a'://Append
			arquivo = fopen(caminho,"ab");
			break;
		case 'f':
			arquivo = fopen(caminho, "r+b");
			break;
		default:
			arquivo = NULL;
			break;

	}

	if (arquivo == NULL) {
		system("cls");
		printf("\n N�o foi poss�vel abrir o arquivo, tente novamente \n");
		system("pause");
		acessarSistema();
	}

	return arquivo;

}

FILE* abreArquivoDeposito(char modo) {

	FILE *arquivo;
	char caminho [30] = "depositos.txt";
	switch (modo) {

		case 'w'://Escrever
			arquivo = fopen(caminho,"wb");
			break;
		case 'r'://Ler
			arquivo = fopen(caminho,"rb");
			break;
		case 'a'://Append
			arquivo = fopen(caminho,"ab");
			break;
		case 'f':
			arquivo = fopen(caminho, "r+b");
			break;
		default:
			arquivo = NULL;
			break;

	}

	return arquivo;

}


FILE* abreArquivoSaque(char modo) {

	FILE *arquivo;
	char caminho [30] = "saques.txt";
	switch (modo) {

		case 'w'://Escrever
			arquivo = fopen(caminho,"wb");
			break;
		case 'r'://Ler
			arquivo = fopen(caminho,"rb");
			break;
		case 'a'://Append
			arquivo = fopen(caminho,"ab");
			break;
		case 'f':
			arquivo = fopen(caminho, "r+b");
			break;
		default:
			arquivo = NULL;
			break;

	}

	return arquivo;

}

FILE* abreArquivoTransferencia(char modo) {

	FILE *arquivo;
	char caminho [30] = "transferencias.txt";
	switch (modo) {

		case 'w'://Escrever
			arquivo = fopen(caminho,"wb");
			break;
		case 'r'://Ler
			arquivo = fopen(caminho,"rb");
			break;
		case 'a'://Append
			arquivo = fopen(caminho,"ab");
			break;
		case 'f':
			arquivo = fopen(caminho, "r+b");
			break;
		default:
			arquivo = NULL;
			break;

	}


	return arquivo;

}

void fecharArquivo(FILE *arquivo) {
	fclose(arquivo);
}


