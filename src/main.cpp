/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klee <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/07 14:41:32 by klee              #+#    #+#             */
/*   Updated: 2018/06/07 14:41:33 by klee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Game.hpp"
#include <ncurses.h>
#include "../inc/Player.hpp"


int		main(){
	Game SpaceInvaders;

	SpaceInvaders.NewGame();

	return 0;
}
