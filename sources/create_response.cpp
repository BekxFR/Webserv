/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_response.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgruson <mgruson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 17:55:04 by nflan             #+#    #+#             */
/*   Updated: 2023/04/14 15:53:11 by mgruson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server_response.hpp"

void	server_response::createResponse(server_configuration * server)
{
	enum	status { INFO, SUCCESS, REDIRECTION, CLIENT, SERVER };
	int	n = 0;
	int	tmp = _status_code / 100;
	for (; n != tmp && n < 5; n++) {}
	switch (n)
	{
		case INFO:
		{
			case 100:
			{
				break;
			}
			case 101:
			{
				break;
			}
			break;
		}
		case SUCCESS:
		{
			// std::cout << "JE SUIS DANS SUCCESS" << std::endl;
			case 200:
			{
				break;
			}
			case 201:
			{
				break;
			}
			case 202:
			{
				break;
			}
			case 203:
			{
				break;
			}
			case 204:
			{
				break;
			}
			case 205:
			{
				break;
			}
			case 206:
			{
				break;
			}
			break;
		}
		case REDIRECTION:
		{
			case 300:
			{
				break;
			}
			case 301:
			{
				break;
			}
			case 302:
			{
				break;
			}
			case 303:
			{
				break;
			}
			case 304:
			{
				break;
			}
			case 305:
			{
				break;
			}
			case 307:
			{
				break;
			}
		}
		case CLIENT:
		{
			case 400:
			{
				break;
			}
			case 401:
			{
				break;
			}
			case 402:
			{
				break;
			}
			case 403:
			{
				break;
			}
			case 404:
			{
				break;
			}
			case 405:
			{
				break;
			}
			case 406:
			{
				break;
			}
			case 407:
			{
				break;
			}
			case 408:
			{
				break;
			}
			case 409:
			{
				break;
			}
			case 410:
			{
				break;
			}
			case 411:
			{
				break;
			}
			case 412:
			{
				break;
			}
			case 413:
			{
				break;
			}
			case 414:
			{
				break;
			}
			case 415:
			{
				break;
			}
			case 416:
			{
				break;
			}
			case 417:
			{
				break;
			}
		}
		case SERVER:
		{
			case 500:
			{
				break;
			}
			case 501:
			{
				break;
			}
			case 502:
			{
				break;
			}
			case 503:
			{
				break;
			}
			case 504:
			{
				break;
			}
			case 505:
			{
				break;
			}
		}
		default:
			std::cout << "default" << std::endl;
	}
}
