#include "Game.h"
#include "Constantes.h"

void Game::loadTextures()
{
	m_textureManager.loadTexture("unite","media/res/SpriteSheetUnite.png");
	m_textureManager.getRef("unite").setSmooth(true);
	m_textureManager.loadTexture("interface", "media/res/Interface.png");
	m_textureManager.loadTexture("interfaceBarre", "media/res/InterfaceBarre.png");
	m_textureManager.loadTexture("tile", "media/res/Tile.png");
	m_textureManager.loadTexture("ressource", "media/res/Ressource.png");
	m_textureManager.loadTexture("filtre", "media/res/Filtre.png");
}

void Game::loadSprites()
{
	m_spriteManager.loadSprite("interface", m_textureManager.getRef("interface"), 800, 200, 0, 0);
	m_spriteManager.getRef("interface").setPosition(0, WIN_HEIGTH + 156);
	m_spriteManager.getRef("interface").scale(1.25, 1.25);

	m_spriteManager.loadSprite("filtre selection", m_textureManager.getRef("filtre"), 128, 128, 0, 0);
	m_spriteManager.loadSprite("filtre attaque", m_textureManager.getRef("filtre"), 128, 128, 1, 0);
	m_spriteManager.loadSprite("filtre deplacement", m_textureManager.getRef("filtre"), 128, 128, 0, 1);
	m_spriteManager.loadSprite("filtre agi", m_textureManager.getRef("filtre"), 128, 128, 1, 1);

	m_spriteManager.loadSprite("interfaceBarre", m_textureManager.getRef("interfaceBarre"), 800, 60, 0, 0);
	m_spriteManager.getRef("interfaceBarre").setPosition(0, 0);
	m_spriteManager.getRef("interfaceBarre").scale(1.25, 1.25);

	m_spriteManager.loadSprite("petrole",m_textureManager.getRef("ressource"),32,32,0,0);
	m_spriteManager.loadSprite("vivre", m_textureManager.getRef("ressource"), 32, 32, 1, 0);
	m_spriteManager.loadSprite("metal", m_textureManager.getRef("ressource"), 32, 32, 0,1);
	m_spriteManager.loadSprite("void", m_textureManager.getRef("ressource"), 32, 32, 1, 1);

	m_spriteManager.loadSprite("plaine", m_textureManager.getRef("tile"), 128, 128, 0, 0);
	m_spriteManager.loadSprite("plage", m_textureManager.getRef("tile"), 128, 128, 1, 0);
	m_spriteManager.loadSprite("montagne", m_textureManager.getRef("tile"), 128, 128, 2, 0);
	m_spriteManager.loadSprite("mer", m_textureManager.getRef("tile"), 128, 128, 0, 1);
	m_spriteManager.loadSprite("foret", m_textureManager.getRef("tile"), 128, 128, 1,1);
	m_spriteManager.loadSprite("marais", m_textureManager.getRef("tile"), 128, 128, 2, 1);
	m_spriteManager.loadSprite("coline", m_textureManager.getRef("tile"), 128, 128, 0, 2);
	m_spriteManager.loadSprite("ville", m_textureManager.getRef("tile"), 128, 128, 1, 2);
	m_spriteManager.loadSprite("ruine", m_textureManager.getRef("tile"), 128, 128, 2, 2);

	m_spriteManager.loadSprite("infanterie", m_textureManager.getRef("unite"), 128, 128, 0, 0);
	m_spriteManager.loadSprite("soldat", m_textureManager.getRef("unite"), 128, 128, 1, 0);
	m_spriteManager.loadSprite("soldat_arme", m_textureManager.getRef("unite"), 128, 128, 2, 0);
	m_spriteManager.loadSprite("demolisseur", m_textureManager.getRef("unite"), 128, 128, 3, 0);

	m_spriteManager.loadSprite("maritime", m_textureManager.getRef("unite"), 128, 128, 0, 1);

	m_spriteManager.loadSprite("motorise", m_textureManager.getRef("unite"), 128, 128, 0, 2);
	m_spriteManager.loadSprite("artillerie", m_textureManager.getRef("unite"), 128, 128, 1, 2);

	m_spriteManager.loadSprite("aerien", m_textureManager.getRef("unite"), 128, 128, 0, 3);
	
}

Game::Game()
	: meteo(&m_window), menu_p(&m_window)
{
	brouillardDeGuerre = true;
	m_uniteSelectionne = NULL;
	m_batimentSelectionne = NULL;
	m_tour = 0;
	std::cout << "Chargement des textures ..." << std::endl;
    loadTextures();
	std::cout << "Chargement des textures termine" << std::endl;
	std::cout << "Chargement des sprites ..." << std::endl;
	loadSprites();
	std::cout << "Chargement des sprites termine" << std::endl;
	m_map = Map();
	m_minimap = Minimap(&m_map);
	m_interface = Interface();
    m_window.create(sf::VideoMode(WIN_WIDTH,WIN_HEIGTH), "Jeu de Strategie", sf::Style::Close);
	centreImage.x = MAP_WIDTH / 2; centreImage.y = MAP_HEIGTH / 2;

	if (MAP_WIDTH % 2 == 0)
		c_view[0] = (MAP_WIDTH * SPRITE) / 2;
	else
		c_view[0] = ((MAP_WIDTH-1) * SPRITE) / 2;
	if (MAP_HEIGTH % 2 == 0)
		c_view[1] = (MAP_HEIGTH * SPRITE) / 2;
	else
		c_view[1] = ((MAP_HEIGTH - 1) * SPRITE) / 2;

    m_view = sf::View(sf::Vector2f((float)c_view[0],(float)c_view[1]),sf::Vector2f((float)WIN_WIDTH,(float)WIN_HEIGTH));
    //m_view.zoom(SPRITE >> 6);
    m_window.setFramerateLimit(60);

	m_players[0] = new Player(sf::Color(127,127,127));
	m_players[1] = new Player(sf::Color(0, 127, 127));
	m_nbJoueur = 2;

	m_numJoueurActif = 0;
	m_playerActif = m_players[m_numJoueurActif];
	m_players[0]->decouvre();

	gameState = 0;
}

void Game::render()
{
	if (gameState == 0) {
		menu_p.render();
	}
	if (gameState == 1) {
		//Render de la map et des entites (batiments et unite) 
		m_view.setCenter((float)c_view[0], (float)c_view[1]);
		m_window.setView(m_view);
		if (brouillardDeGuerre) {
			m_map.render(&m_window, &m_spriteManager, m_playerActif);
		}
		else {
			m_map.render(&m_window, &m_spriteManager);
		}
		for (int i = 0; i < m_nbJoueur; i++) {
			if (brouillardDeGuerre) {
				m_players[i]->render(&m_window, &m_spriteManager, m_playerActif);
			}
			else
				m_players[i]->render(&m_window, &m_spriteManager);
		}

		if (m_uniteSelectionne != NULL) {
			surbrillanceCaseDeplacement();
		}

		/*
		//Croix rouge au milieu de l'�cran
		sf::RectangleShape shape(sf::Vector2f(2, 10));
		shape.setPosition(c_view[0]-1,c_view[1]-5);
		shape.setFillColor(sf::Color(255, 0, 0));
		m_window.draw(shape);
		sf::RectangleShape shape2(sf::Vector2f(10, 2));
		shape2.setPosition(c_view[0]-5, c_view[1]-1);
		shape2.setFillColor(sf::Color(255, 0, 0));
		m_window.draw(shape2);
		*/

		// Render de l'interface
		m_window.setView(m_viewInterface);
		m_interface.render(&m_window, &m_spriteManager);


		// Render de la minimap
		m_window.setView(m_viewMinimap);
		m_minimap.render(&m_window);
		if (m_minimap.getUniteMode()) {
			m_minimap.renderPlayer(&m_window, m_players, m_nbJoueur);
		}

		if (weather_clock.getElapsedTime().asMilliseconds() < 16) { // 60 fps
			meteo.render();
			weather_clock.restart();
		}
	}
	
}

void Game::clic(int x, int y) {

	if (gameState == 0) {		
		if (menu_p.hitQuitter(x, y)) {
			m_window.close();
		}

		if (menu_p.hitNouvellePartie(x, y)) {
			gameState = 1;
		}
	}

	if (gameState == 1) {
		// Zone clique jeu
		if (testClicZoneJeu(x, y)) {
			clicZoneJeu(x, y);
		}// Zone clique interface
		else {
			clicInterface(x, y);
		}
	}
}

bool Game::testClicZoneJeu(int x, int y) {
	// Zone principale
	return y < 443;
}

sf::Vector2i Game::definitionCaseClique(int x, int y) {
	sf::Vector2i caseClique(-1, -1);
	// Variable � modifier pour g�rer le zoom
	int tailleCaseSurEcran = SPRITE;
	int nbCaseAfficheParLigne = round(WIN_WIDTH / tailleCaseSurEcran);
	int nbCaseAfficheParColonne = round((float)(WIN_HEIGTH - INTERFACE_HEIGTH ) / (float)tailleCaseSurEcran);
	int decalageX = round((WIN_WIDTH - (nbCaseAfficheParLigne * tailleCaseSurEcran)) / 2);
	int decalageY = INTERFACE_HAUT_HEIGHT;

	// D�fini les zones de clics des cases
	for (int i = 0; i < nbCaseAfficheParLigne; i++) {
		for (int j = 0; j < nbCaseAfficheParColonne; j++) {
			if (x >= decalageX + i*tailleCaseSurEcran && x < decalageX + (i + 1)*tailleCaseSurEcran 
				&& y < decalageY + (j + 1)*tailleCaseSurEcran  && y >= (j*tailleCaseSurEcran) + decalageY) {
				caseClique.y = centreImage.y + (-nbCaseAfficheParLigne / 2 + 1 + j);
				caseClique.x = centreImage.x + (-nbCaseAfficheParColonne + 1 / 2 + i);
				break;
			}
		}
	}
	return caseClique;
}

void Game::actionUnite(sf::Vector2i caseClique) {
	for (int i = 0; i < m_deplacement.size(); i++) {
		if (caseClique.x == m_deplacement[i].x / SPRITE && caseClique.y == m_deplacement[i].y / SPRITE) {
			m_uniteSelectionne->seDeplace(caseClique.x, caseClique.y, &m_window, m_playerActif->getColor(), &m_spriteManager);
			m_playerActif->decouvre();
		}
	}
	for (int i = 0; i < m_attaque.size(); i++) {
		if (caseClique.x == m_attaque[i].x / SPRITE && caseClique.y == m_attaque[i].y / SPRITE) {
			UniteArmee *unite = (UniteArmee*)m_uniteSelectionne;
			for (int j = 0; j < m_nbJoueur; j++) {
				if (!j == m_numJoueurActif) {
					for (int k = 0; k < m_players[j]->getNombreUnite(); k++) {
						if (m_players[j]->getUnite(k)->getCoordX() == caseClique.x && m_players[j]->getUnite(k)->getCoordY() == caseClique.y) {
							int ecartUniteX = unite->getCoordX() - m_players[j]->getUnite(k)->getCoordX();
							int ecartUniteY = unite->getCoordY() - m_players[j]->getUnite(k)->getCoordY();
							int distance = abs(ecartUniteX) + abs(ecartUniteY);
							std::cout << distance << std::endl;

							if (distance > unite->getRangeMax()) {
								deplacementAutoPourAttaque(ecartUniteX, ecartUniteY, distance, unite, m_players[j]->getUnite(k)->getCoordX(), m_players[j]->getUnite(k)->getCoordY());
								m_playerActif->decouvre();
							}
							unite->attaque(m_players[j]->getUnite(k));
							
							if (m_players[j]->getUnite(k)->estDetruit()) {
								m_players[j]->detruireUnite(k);
							}
						}
					}
				}
			}
		}
	}
}

void Game::deplacementAutoPourAttaque(int ecartX, int ecartY, int distance, UniteArmee* unite, int posXEnnemi, int posYEnnemi) {
	int deplacementX = 0, deplacementY = 0;
	while (distance > unite->getRangeMax()) {
		if (abs(ecartX) < abs(ecartY)) {
			if (posYEnnemi < unite->getCoordY() + deplacementY) {
				deplacementY--;
			}
			else {
				deplacementY++;
			}
		}
		else {
			if (posXEnnemi < unite->getCoordX() + deplacementX) {
				deplacementX--;
			}
			else {
				deplacementX++;
			}
		}
		ecartX = unite->getCoordX() + deplacementX - posXEnnemi;
		ecartY = unite->getCoordY() + deplacementY - posYEnnemi;
		distance = abs(ecartX) + abs(ecartY);
	}
	unite->seDeplace(unite->getCoordX() + deplacementX, unite->getCoordY() + deplacementY, &m_window, m_playerActif->getColor(),&m_spriteManager);
}

void Game::clicZoneJeu(int x, int y) {
	sf::Vector2i caseClique = definitionCaseClique(x, y);

	if (m_uniteSelectionne != NULL && m_uniteSelectionne->peutAgir()) {
		actionUnite(caseClique);
		deselection();
	}
	else {
		selection(caseClique, x, y);
	}
}

void Game::clicUnite(int x, int y, Unite *unite) {
	m_interface.setModeUnite();
	m_uniteSelectionne = unite;
	definitionCase();
}

void Game::clicInterface(int x, int y) {
	if (x < 43 && 6 < x) {
		if (y < 485 && 465 < y) {
			m_minimap.changeModeTopo();
		}
		else if (y < 530 && 510 < y) {
			m_minimap.changeModeRessource();
		}
		else if (y < 575 && 555 < y) {
			m_minimap.changeModeUnite();
		}
	}
	else if (x < 233 && 64 < x && y < 485 && 460 < y) {
		finTour();
	}
	else if (x < 312 && 266 < x && y < 575 && 490 < y) {
		std::cout << "Fl�che gauche " << std::endl;
	}
	else if (x < 786 && 740 < x && y < 575 && 490 < y) {
		std::cout << "Fl�che droite " << std::endl;
	}
	else if (x < 395 && 261 < x && y < 30 && 4 < y) {
		std::cout << "Technologies " << std::endl;
	}
	else if (x < 544 && 410 < x && y < 30 && 4 < y) {
		std::cout << "Construction " << std::endl;
	}
	else if (x < 744 && 710 < x && y < 31 && 2 < y) {
		std::cout << "Options " << std::endl;
	}
	else if (x < 787 && 753 < x && y < 31 && 2 < y) {
		std::cout << "Exit " << std::endl;
		m_window.close();
	}
}

Player* Game::getPlayerActif() 
{
	return m_playerActif;
}

void Game::joueurSuivant() {
	m_deplacement.clear();
	m_attaque.clear();
	m_numJoueurActif++;
	if (m_numJoueurActif >= m_nbJoueur) {
		m_tour++;
		m_numJoueurActif = 0;
	}
	std::cout << "c'est au tour du joueur " << m_numJoueurActif << std::endl;
	m_playerActif = m_players[m_numJoueurActif];
	m_playerActif->decouvre();
}

void Game::finTour() {
	m_batimentSelectionne = NULL;
	m_uniteSelectionne = NULL;
	m_playerActif->update();
	joueurSuivant();
	// �a pourrait �tre sympa d'afficher en plus "C'est au tour de joueur : "
	
}

void Game::definitionCase() {
	m_attaque.clear();
	m_deplacement.clear();
	int xOrig = m_uniteSelectionne->getCoordX();
	int yOrig = m_uniteSelectionne->getCoordY();
	int nbCase = m_uniteSelectionne->getDeplacementMax();
	if (m_uniteSelectionne->getStockRessActuel() < m_uniteSelectionne->getDeplacementMax()) {
		nbCase = m_uniteSelectionne->getStockRessActuel();
	}
	// Unite armee
	if (m_uniteSelectionne->isArmee()) {
		UniteArmee* unite = (UniteArmee*)m_uniteSelectionne;
		if (unite->peutAttaquer()) {
			// Unite armee classique
			if (unite->getPeutBougerEtAttaquer()) {
				definitionCaseUniteArmee(xOrig, yOrig, nbCase, nbCase + unite->getRangeMax());
			}
			// Unite armee de type artillerie et cuirass�
			else {
				definitionCaseUniteDistance(xOrig, yOrig, nbCase, unite->getRangeMin(), unite->getRangeMax());
			}
		}
		else
			definitionCaseUnite(xOrig, yOrig, nbCase);
	}
	//Unite non armee
	else {
		definitionCaseUnite(xOrig, yOrig, nbCase);
	}
	
}

void Game::definitionCaseUnite(int xOrig, int yOrig, int nbCase) {
	for (int i = xOrig; i < xOrig + nbCase; i++) {
		if (i >= MAP_WIDTH) break;
		for (int j = yOrig; (i - xOrig) + (j - yOrig) < nbCase; j++) {
			if (j >= MAP_HEIGTH) break;
			if (!(i == xOrig && j == yOrig)) {
				if (!testUniteAlliee(i, j)) {
					if (!testEntiteEnnemie(i, j)) {
						if (testUniteSelectionneTypeCase(i, j)) {
							m_deplacement.push_back(sf::Vector2f(i*SPRITE, j*SPRITE));
						}
					}
				}
			}
		}
		for (int j = yOrig; (i - xOrig) + (yOrig - j) < nbCase; j--) {
			if (j < 0) break;
			if (!(i == xOrig && j == yOrig) && j != yOrig) {
				if (!testUniteAlliee(i, j)) {
					if (!testEntiteEnnemie(i, j)) {
						if (testUniteSelectionneTypeCase(i, j)) {
							m_deplacement.push_back(sf::Vector2f(i*SPRITE, j*SPRITE));
						}
					}
				}
			}
		}
	}
	for (int i = xOrig - 1; i > xOrig - nbCase || i > 0; i--) {
		if (i < 0) break;
		for (int j = yOrig; (xOrig - i) + (j - yOrig) < nbCase; j++) {
			if (j >= MAP_HEIGTH) break;
			if (!(i == xOrig && j == yOrig)) {
				if (!testUniteAlliee(i, j)) {
					if (!testEntiteEnnemie(i, j)) {
						if (testUniteSelectionneTypeCase(i, j)) {
							m_deplacement.push_back(sf::Vector2f(i*SPRITE, j*SPRITE));
						}
					}
				}
			}
		}
		for (int j = yOrig; (xOrig - i) + (yOrig - j) < nbCase; j--) {
			if (j < 0) break;
			if (!(i == xOrig && j == yOrig) && j != yOrig) {
				if (!testUniteAlliee(i, j)) {
					if (!testEntiteEnnemie(i, j)) {
						if (testUniteSelectionneTypeCase(i, j)) {
							m_deplacement.push_back(sf::Vector2f(i*SPRITE, j*SPRITE));
						}
					}
				}
			}
		}
	}
}

void Game::definitionCaseUniteArmee(int xOrig, int yOrig, int nbCase, int nbCaseAttaque) {

	for (int i = xOrig; i < xOrig + nbCaseAttaque; i++) {
		if (i >= MAP_WIDTH) break;
		for (int j = yOrig; (i - xOrig) + (j - yOrig) < nbCaseAttaque; j++) {
			if (j >= MAP_HEIGTH) break;
			if (!(i == xOrig && j == yOrig)) {
				if (!testUniteAlliee(i, j)) {
					if (!testEntiteEnnemie(i, j)) {
						if (testUniteSelectionneTypeCase(i, j) && (i - xOrig) + (j - yOrig) < nbCase) {
							m_deplacement.push_back(sf::Vector2f(i*SPRITE, j*SPRITE));
						}
					}
					else {
						m_attaque.push_back(sf::Vector2f(i*SPRITE, j*SPRITE));
					}
				}
			}
		}
		for (int j = yOrig; (i - xOrig) + (yOrig - j) < nbCaseAttaque; j--) {
			if (j < 0) break;
			if (!(i == xOrig && j == yOrig) && j != yOrig) {
				if (!testUniteAlliee(i, j)) {
					if (!testEntiteEnnemie(i, j)) {
						if (testUniteSelectionneTypeCase(i, j) && (i - xOrig) + (yOrig - j) < nbCase) {
							m_deplacement.push_back(sf::Vector2f(i*SPRITE, j*SPRITE));
						}
					}
					else {
						m_attaque.push_back(sf::Vector2f(i*SPRITE, j*SPRITE));
					}
				}
			}
		}
	}
	for (int i = xOrig - 1; i > xOrig - nbCaseAttaque || i > 0; i--) {
		if (i < 0) break;
		for (int j = yOrig; (xOrig - i) + (j - yOrig) < nbCaseAttaque; j++) {
			if (j >= MAP_HEIGTH) break;
			if (!(i == xOrig && j == yOrig)) {
				if (!testUniteAlliee(i, j)) {
					if (!testEntiteEnnemie(i, j)) {
						if (testUniteSelectionneTypeCase(i, j) && (xOrig - i) + (j - yOrig) < nbCase) {
							m_deplacement.push_back(sf::Vector2f(i*SPRITE, j*SPRITE));
						}
					}
					else {
						m_attaque.push_back(sf::Vector2f(i*SPRITE, j*SPRITE));
					}
				}
			}
		}
		for (int j = yOrig; (xOrig - i) + (yOrig - j) < nbCaseAttaque; j--) {
			if (j < 0) break;
			if (!(i == xOrig && j == yOrig) && j != yOrig) {
				if (!testUniteAlliee(i, j)) {
					if (!testEntiteEnnemie(i, j)) {
						if (testUniteSelectionneTypeCase(i, j) && (xOrig - i) + (yOrig - j) < nbCase) {
							m_deplacement.push_back(sf::Vector2f(i*SPRITE, j*SPRITE));
						}
					}
					else {
						m_attaque.push_back(sf::Vector2f(i*SPRITE, j*SPRITE));
					}
				}
			}
		}
	}
}

void Game::definitionCaseUniteDistance(int xOrig, int yOrig, int nbCase, int rangeMin, int rangeMax) {
	// Definition case deplacable
	for (int i = xOrig; i < xOrig + nbCase; i++) {
		if (i >= MAP_WIDTH) break;
		for (int j = yOrig; (i - xOrig) + (j - yOrig) < nbCase; j++) {
			if (j >= MAP_HEIGTH) break;
			if (!(i == xOrig && j == yOrig)) {
				if (!testUniteAlliee(i, j)) {
					if (!testEntiteEnnemie(i, j)) {
						if (testUniteSelectionneTypeCase(i, j)) {
							m_deplacement.push_back(sf::Vector2f(i*SPRITE, j*SPRITE));
						}
					}
				}
			}
		}
		for (int j = yOrig; (i - xOrig) + (yOrig - j) < nbCase; j--) {
			if (j < 0) break;
			if (!(i == xOrig && j == yOrig) && j != yOrig) {
				if (!testUniteAlliee(i, j)) {
					if (!testEntiteEnnemie(i, j)) {
						if (testUniteSelectionneTypeCase(i, j)) {
							m_deplacement.push_back(sf::Vector2f(i*SPRITE, j*SPRITE));
						}
					}
				}
			}
		}
	}
	for (int i = xOrig - 1; i > xOrig - nbCase || i > 0; i--) {
		if (i < 0) break;
		for (int j = yOrig; (xOrig - i) + (j - yOrig) < nbCase; j++) {
			if (j >= MAP_HEIGTH) break;
			if (!(i == xOrig && j == yOrig)) {
				if (!testUniteAlliee(i, j)) {
					if (!testEntiteEnnemie(i, j)) {
						if (testUniteSelectionneTypeCase(i, j)) {
							m_deplacement.push_back(sf::Vector2f(i*SPRITE, j*SPRITE));
						}
					}
				}
			}
		}
		for (int j = yOrig; (xOrig - i) + (yOrig - j) < nbCase; j--) {
			if (j < 0) break;
			if (!(i == xOrig && j == yOrig) && j != yOrig) {
				if (!testUniteAlliee(i, j)) {
					if (!testEntiteEnnemie(i, j)) {
						if (testUniteSelectionneTypeCase(i, j)) {
							m_deplacement.push_back(sf::Vector2f(i*SPRITE, j*SPRITE));
						}
					}
				}
			}
		}
	}

	// Definition case attaquable
	for (int i = xOrig + rangeMin; i < xOrig + rangeMax; i++) {
		if (i >= MAP_WIDTH) break;
		for (int j = yOrig + rangeMin; (i - xOrig) + (j - yOrig) < rangeMax; j++) {
			if (j >= MAP_HEIGTH) break;
			if (!(i == xOrig && j == yOrig)) {
				if (testEntiteEnnemie(i, j)) {
					m_attaque.push_back(sf::Vector2f(i*SPRITE, j*SPRITE));
				}
			}
		}
		for (int j = yOrig - rangeMin; (i - xOrig) + (yOrig - j) < rangeMax; j--) {
			if (j < 0) break;
			if (!(i == xOrig && j == yOrig) && j != yOrig) {
				if (testEntiteEnnemie(i, j)) {
					m_attaque.push_back(sf::Vector2f(i*SPRITE, j*SPRITE));
				}
			}
		}
	}
	for (int i = xOrig - rangeMin - 1; i > xOrig - rangeMax || i > 0; i--) {
		if (i < 0) break;
		for (int j = yOrig + rangeMin; (xOrig - i) + (j - yOrig) < rangeMax; j++) {
			if (j >= MAP_HEIGTH) break;
			if (!(i == xOrig && j == yOrig)) {
				if (testEntiteEnnemie(i, j)) {
					m_attaque.push_back(sf::Vector2f(i*SPRITE, j*SPRITE));
				}
			}
		}
		for (int j = yOrig - rangeMin; (xOrig - i) + (yOrig - j) < rangeMax; j--) {
			if (j < 0) break;
			if (!(i == xOrig && j == yOrig) && j != yOrig) {
				if (testEntiteEnnemie(i, j)) {
					m_attaque.push_back(sf::Vector2f(i*SPRITE, j*SPRITE));
				}
			}
		}
	}
}

void Game::surbrillanceCaseDeplacement() {
	sf::Sprite sprite = m_spriteManager.getRef("filtre attaque");
	for (int i = 0; i < m_attaque.size(); i++) {
		sprite.setPosition(m_attaque[i]);
		m_window.draw(sprite);
	}

	sprite = m_spriteManager.getRef("filtre deplacement");
	for (int i = 0; i < m_deplacement.size(); i++) {
		sprite.setPosition(m_deplacement[i]);
		m_window.draw(sprite);
	}
	sprite = m_spriteManager.getRef("filtre selection");
	sprite.setPosition(sf::Vector2f(m_uniteSelectionne->getCoordX()*SPRITE, m_uniteSelectionne->getCoordY()*SPRITE));
	m_window.draw(sprite);
}

bool Game::testUniteAlliee(int x, int y) {
	for (int i = 0; i < getPlayerActif()->getNombreUnite(); i++) {
		if (m_playerActif->getUnite(i)->getCoordX() == x && m_playerActif->getUnite(i)->getCoordY() == y) {
			return true;
		}
	}
	return false;
}

bool Game::testEntiteEnnemie(int x, int y) {
	for (int j = 0; j < m_nbJoueur; j++) {
		if (!j == m_numJoueurActif) {
			for (int i = 0; i < m_players[j]->getNombreUnite(); i++) {
				if (m_players[j]->getUnite(i)->getCoordX() == x && m_players[j]->getUnite(i)->getCoordY() == y) {
					return true;
				}
			}
			for (int i = 0; i < m_players[j]->getNombreBatiment(); i++) {
				if (m_players[j]->getBatiment(i)->getCoordX() == x && m_players[j]->getBatiment(i)->getCoordY() == y) {
					return true;
				}
			}
		}
	}
	return false;
}

bool Game::testUniteSelectionneTypeCase(int x, int y) {
	TypeCase caseActuelle = m_map.getTile(x, y).getTypeCase();
	if (caseActuelle == TypeCase::MONTAGNE && !m_uniteSelectionne->isAerienne())
		return false;
	if (m_uniteSelectionne->isMaritime()) {
		if (caseActuelle == TypeCase::MER || caseActuelle == TypeCase::PLAGE) 
			return true;
	}
	else if (m_uniteSelectionne->isInfanterie() || m_uniteSelectionne->isMotorise()) {
		if (caseActuelle == TypeCase::MER)
			return false;
	}
	return true;
}

void Game::selection(sf::Vector2i caseClique, int x, int y) {
	if(m_uniteSelectionne == NULL && m_batimentSelectionne == NULL){
		for (int i = 0; i < m_playerActif->getNombreUnite(); i++) {
			if (m_playerActif->getUnite(i)->getCoordX() == caseClique.x && m_playerActif->getUnite(i)->getCoordY() == caseClique.y && m_playerActif->getUnite(i)->peutAgir()) {
				clicUnite(x, y, m_playerActif->getUnite(i));
				std::cout << "Nom : " << m_uniteSelectionne->getNom() << std::endl;
				std::cout << "Description : " << m_uniteSelectionne->getDescription() << std::endl;
				std::cout << "PV : " << m_uniteSelectionne->getPvRestant()<< "/" << 10 << std::endl;
				std::cout << "Ressource : " << m_uniteSelectionne->getStockRessActuel()<<"/"<< m_uniteSelectionne->getStockMaxRess() <<std::endl;
				if (m_uniteSelectionne->isArmee()) {
					UniteArmee* unite = (UniteArmee*)m_uniteSelectionne;
					std::cout << "Munition : " << unite->getStockMunActuel() << "/" << unite->getStockMaxMun() << std::endl;
					std::cout << "Port�e entre " << unite->getRangeMin() << " et " << unite->getRangeMax() << std::endl;
				}
			}
		}
		if (m_uniteSelectionne == NULL) {
			for (int i = 0; i < m_playerActif->getNombreBatiment(); i++) {
				if (m_playerActif->getBatiment(i)->getCoordX() == caseClique.x && m_playerActif->getBatiment(i)->getCoordY() == caseClique.y) {
					std::cout << "Batiment clique" << std::endl;
				}
			}
		}
	}
}

void Game::deselection() {
	m_uniteSelectionne = NULL;
	m_batimentSelectionne = NULL;
}

int Game::getState() {
	return gameState;
}