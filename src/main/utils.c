#include "header.h"

void ft_putdino(char *str) {
	char	base[] = "groar";
	int		len = ft_strlen(base);

	for (int i = 0; str[i]; i++) {
		int mod = str[i] % len;
		ft_putchar_fd(base[mod], 1);
	}
}

char *join_with_separator(char *str1, char *str2, char sep) {
	if (!str1 || !str2)
		return NULL;

	int len_s1 = ft_strlen(str1);
	int len_s2 = ft_strlen(str2);
	int len =  len_s1 + len_s2 + 1;

	char *result = malloc((len + 1) * sizeof(char));
	if (!result)
		return NULL;
	for (int i = 0; i < len_s1; i++) {
		result[i] = str1[i];
	}
	result[len_s1] = sep;
	for (int i = 0; i < len_s2; i++) {
		result[len_s1 + i + 1] = str2[i];
	}
	result[len] = '\0';
	return result;
}

char *clean_join(char *origin, const char *to_join) {
	char *tmp = origin;
	char *res = ft_strjoin(origin, to_join);
	free(tmp);
	return res;
}

unsigned int get_size_nbr(int nb) {
	int result = 0;

	if (nb == INT_MIN)
		return 11;
	if (nb == 0)
		return 1;

	if (nb < 0) {
		nb *= -1;
		result++;
	}

	while (nb != 0) {
		result++;
		nb /= 10;
	}

	return result;
}

int round_split(int a, int b) {
	if (b == 0)
		return 0;
	int result = a / b;
	if ((double)(a) / (double)(b) > result)
		result++;
	return result;
}
