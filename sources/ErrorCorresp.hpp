/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorCorresp.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 13:07:31 by nflan             #+#    #+#             */
/*   Updated: 2023/04/25 19:25:15 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORCORRESP_HPP
#define ERRORCORRESP_HPP

#include "lib.hpp"

class	ErrorCorresp
{
	public:
		ErrorCorresp();
		~ErrorCorresp();

		std::map<std::string, std::string>	getCorresp();

	private:
		std::map<std::string, std::string>	_Corresp;

		ErrorCorresp(const ErrorCorresp &);
		ErrorCorresp& operator=(const ErrorCorresp &);
};

#endif
