#include "../include/woody.h"

int		ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t			i;
	int				result;
	unsigned char	*tmp1;
	unsigned char	*tmp2;

	i = 0;
	tmp1 = (unsigned char *)s1;
	tmp2 = (unsigned char *)s2;
	while (tmp1[i] != '\0' && tmp2[i] != '\0' && i < n)
	{
		result = tmp1[i] - tmp2[i];
		if (result != 0)
			return (result);
		i++;
	}
	return (0);
}

int		ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s2)
	{
		if ((unsigned char)*s1 != (unsigned char)*s2)
			break ;
		s1++;
		s2++;
	}
	return ((int)(*s1 - *s2));
}

int			return_error(char *text)
{
	if (!text)
		perror("woody_woodpacker");
	else
		dprintf(2, "woody_woodpacker: %s\n", text);
	return (1);
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t		i;
	char		*d;
	const char	*s;

	i = 0;
	d = (char *)dst;
	s = (const char *)src;
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return ((void *)d);
}
