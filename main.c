#include <fcntl.h>
#include <stdio.h>
#include "get_next_line.h"

int	main(int argc, char **argv)
{
	int		fd;
	char	*line;

	if (argc != 2)
		return (1);
	fd = open(argv[1], O_RDONLY);
	while ((line = get_next_line(fd)) != NULL)
	{
		printf("%s", line);
		free(line);
	}
	close(fd);
	return (0);
}



//int	main(void)
//{
//	char	*line;

//	while ((line = get_next_line(0)) != NULL)
//	{
//		printf("\n[%s]\n", line);
//		free(line);
//	}
//	return (0);
//}

//int main()
//{
//	//char *str = "hello";
//	//char *str2 = "hello";
//	//char *str3 = "hello";

//	printf("%");
//}
