/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   info.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 11:43:12 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/04 15:36:42 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "network.h"
#include <ifaddrs.h>

static void	print_network_ip(void)
{
	struct ifaddrs		*ifaddr;
	struct ifaddrs		*ifa;
	struct sockaddr_in	*sa;
	char				ip[INET_ADDRSTRLEN];

	if (getifaddrs(&ifaddr) == -1)
		return ;
	printf("• Network IPs:\n");
	ifa = ifaddr;
	while (ifa != NULL)
	{
		if (!(ifa->ifa_addr == NULL || ifa->ifa_addr->sa_family != AF_INET))
		{
			sa = (struct sockaddr_in *)ifa->ifa_addr;
			inet_ntop(AF_INET, &(sa->sin_addr), ip, INET_ADDRSTRLEN);
			if (ft_strcmp(ip, "127.0.0.1") != 0)
				printf("  - %s: %s\n", ifa->ifa_name, ip);
		}
		ifa = ifa->ifa_next;
	}
	freeifaddrs(ifaddr);
}

void	print_network_info(int port)
{
	char				hostname[256];

	printf("Server listening on port %d...\n", port);
	if (gethostname(hostname, sizeof(hostname)) == 0)
		printf("• Hostname: %s\n", hostname);
	print_network_ip();
	write(1, "• Public IP ", 15);
	system("curl -s ifconfig.me");
	write(1, "\n", 1);
}
