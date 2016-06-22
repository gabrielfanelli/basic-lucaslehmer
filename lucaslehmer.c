#include <stdio.h>
#include <ctype.h>
#include <gmp.h>

//Calcula a potência usando a expansão binária do expoente
void retorna_potencia(int base, unsigned long long exp, mpz_t mp){

	mpz_t temp;
	mpz_init(temp);
	mpz_set_ui(temp, base);

	while (exp){

		if (exp & 1){ //Se impar faz uma multiplicação extra

			mpz_mul(mp, mp, temp);
		}

		exp /= 2;
		mpz_mul(temp, temp, temp); //Eleva ao quadrado
    }

    mpz_clear(temp);
}

//Implementação do teste Lucas-Lehmer
int lucas_lehmer_test(unsigned long long p, char imprimir){
	mpz_t mp;
	mpz_init(mp);
	mpz_set_ui(mp, 1);

	//Define S0 = 4
	mpz_t s;
	mpz_init(s);
	mpz_set_ui(s, 4);

	int i;

	//Calcula o numero de Mersenne a ser testado
	retorna_potencia(2, p, mp);
	mpz_sub_ui(mp, mp, 1);
	
	for(i = 0; i < p - 2; i++){

		mpz_mul(s, s, s);
		mpz_sub_ui(s, s, 2);
		mpz_mod(s, s, mp);
	}

	//Rotinas de impressão
	if(!(mpz_cmp_ui(s, 0))){
		if(imprimir == 'S'){
			printf("M%llu: ", p);
			mpz_out_str(stdout, 10, mp);
			printf("\n");
		}
		mpz_clear(s);
		mpz_clear(mp);
		return 1;
	}else{
		mpz_clear(s);
		mpz_clear(mp);
		return 0;
	}

}

//Retorna a parte inteira da raiz de p
unsigned long retorna_raiz(unsigned long p){
	unsigned long x, y, aux;

	//Inicializa x e y
	x = p;
	y = (p + 1) / 2;

	while(x != y){
		
		aux = y;
		y = (x*x + p) / (2*x);
		x = aux;
	}

	return x;
}

//Verifica se p é primo procurando por divisores
int verifica_primo(unsigned long p){

	//Se p é par então não é primo
	if(!(p&1)){
		return 0;
	}

	unsigned long n, q;
	q = retorna_raiz(p);

	//Procura possiveis divisores de p
	for(n = 3; n < q; n += 2){
		if(n%q == 0){
			return 0;
		}
	}

	return 1;
}

int main(void){

	unsigned long long p, i;
	char imprimir;
	printf("Digite ate qual p deseja fazer a verificaçao: \n");
	scanf("%llu", &p);
	printf("Deseja imprimir Mp?: s/n\n");
	do{
		scanf("%c", &imprimir);
		imprimir = toupper(imprimir);
	}while(imprimir != 'S' && imprimir != 'N');

	for(i = 2; i <= p; i++){

		//Para p = 2 o teste é pulado
		if(i == 2){
			if(imprimir == 'S'){
				printf("\nM2: 3\n");
			}else{
				printf("\nM2\n");
			}
		}else if(verifica_primo(i) && lucas_lehmer_test(i, imprimir)){
			if(imprimir == 'N'){
				printf("M%llu\n", i );
			}
		}
	}

	printf("\n");
	return 0;
}
