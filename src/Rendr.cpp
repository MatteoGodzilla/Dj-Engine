#include "Rendr.h"

Rendr::Rendr(sf::RenderWindow &w):m_window(w) {
    sf::Texture loader;
    if(loader.loadFromFile("res/texture.png")) {
        m_tex = loader;
    }

    sf::Font font_loader;
    if(font_loader.loadFromFile("res/NotoSans-Regular.ttf")) {
        m_font = font_loader;
    }

    m_time_txt.setFont(m_font);
    m_time_txt.setPosition(0.0,0.0);
    m_time_txt.setFillColor(sf::Color::White);

    m_score_txt.setFont(m_font);
    m_score_txt.setPosition(0.0,30.0);
    m_score_txt.setFillColor(sf::Color::White);

    m_combo_txt.setFont(m_font);
    m_combo_txt.setPosition(0.0,60.0);
    m_combo_txt.setFillColor(sf::Color::White);

    m_mult_txt.setFont(m_font);
    m_mult_txt.setPosition(0.0,90.0);
    m_mult_txt.setFillColor(sf::Color::White);

    //scale setup
    m_scl_start = sf::Vector2f(0.1,0.1);
    m_scl_end = sf::Vector2f(0.2,0.2);
    m_scl_vel = (m_scl_end-m_scl_start)/1.0f;

    m_trayL.setTexture(m_tex);
    m_trayL.setTextureRect(sf::IntRect(1320,0,440,880));
    m_trayL.setOrigin(220,440);
    m_trayL.setRotation(90);
    m_trayL.setScale(0.2,0.2);
    m_trayL.setPosition(382.0,500.0);

    m_trayR.setTexture(m_tex);
    m_trayR.setTextureRect(sf::IntRect(1320,0,440,880));
    m_trayR.setOrigin(220,400);
    m_trayR.setRotation(90);
    m_trayR.setScale(0.2,0.2);
    m_trayR.setPosition(652.0,500.0);

    m_red_click.setTexture(m_tex);
    m_red_click.setTextureRect(sf::IntRect(440,400,440,440));
    m_red_click.setOrigin(220,220);
    m_red_click.setScale(0.2,0.2);
    m_red_click.setPosition(512.0,500.0);

    m_green_click.setTexture(m_tex);
    m_green_click.setTextureRect(sf::IntRect(0,400,440,440));
    m_green_click.setOrigin(220,220);
    m_green_click.setScale(0.2,0.2);
    m_green_click.setPosition(424.0,500.0);

    m_blue_click.setTexture(m_tex);
    m_blue_click.setTextureRect(sf::IntRect(880,400,440,440));
    m_blue_click.setOrigin(220,220);
    m_blue_click.setScale(0.2,0.2);
    m_blue_click.setPosition(600.0,500.0);

    for(int i = 0; i < resolution; ++i){
        m_lanes.push_back(1);
    }

}

void Rendr::clicker() {
    if(m_red)m_red_click.setScale(0.15,0.15);
    else m_red_click.setScale(0.2,0.2);

    if(m_green)m_green_click.setScale(0.15,0.15);
    else m_green_click.setScale(0.2,0.2);

    if(m_blue)m_blue_click.setScale(0.15,0.15);
    else m_blue_click.setScale(0.2,0.2);

    if(m_player_cross == 0) {
        m_green_click.setPosition(338.0,500.0);
        m_blue_click.setPosition(600.0,500.0);
    } else if(m_player_cross == 1) {
        m_green_click.setPosition(426.0,500.0);
        m_blue_click.setPosition(600.0,500.0);
    } else if(m_player_cross == 2) {
        m_green_click.setPosition(426.0,500.0);
        m_blue_click.setPosition(688.0,500.0);
    }

    m_window.draw(m_trayL);
    m_window.draw(m_trayR);
    m_window.draw(m_red_click);
    m_window.draw(m_green_click);
    m_window.draw(m_blue_click);
    m_window.draw(m_time_txt);
    m_window.draw(m_score_txt);
    m_window.draw(m_combo_txt);
    m_window.draw(m_mult_txt);
}

void Rendr::notes(float time,std::vector<Note> &v) {
    for(size_t i = 0 ; i < v.size(); ++i) {
        float dt = v.at(i).getMilli()-time;
        sf::Sprite sprite;
        sprite.setTexture(m_tex);
        int type = v.at(i).getType();
        if(!v.at(i).getRender()){
            if(m_ren_cross == 0){
                v.at(i).setLanMod(0);
            }else if(m_ren_cross == 1){
                v.at(i).setLanMod(1);
            }else if(m_ren_cross == 2){
                v.at(i).setLanMod(2);
            }
        }

        if(type == TAP_R ) {
            sprite.setTextureRect(sf::IntRect(400,0,400,400));
            m_start = sf::Vector2f(512.0,200.0);
            m_end = sf::Vector2f(512.0,500.0);
            m_vel = (m_end-m_start)/1.0f;
        } else if (type == TAP_G || type == SCR_G_UP || type == SCR_G_DOWN || type == SCR_G_ANY) {
            if(type== TAP_G) {
                sprite.setTextureRect(sf::IntRect(0,0,400,400));
            } else if(type == SCR_G_UP) {
                sprite.setTextureRect(sf::IntRect(800,840,400,400));
            } else if(type == SCR_G_DOWN) {
                sprite.setTextureRect(sf::IntRect(400,840,400,400));
            } else if(type == SCR_G_ANY) {
                sprite.setTextureRect(sf::IntRect(0,840,400,400));
            }
            if(v.at(i).getLanMod() == 0){
                //green left
                m_start = sf::Vector2f(436.0,200.0);
                m_end = sf::Vector2f(338.0,500.0);
            }else if(v.at(i).getLanMod() >= 1){
                //green center
                m_start = sf::Vector2f(472.0,200.0);
                m_end = sf::Vector2f(426.0,500.0);
            }
            m_vel = (m_end-m_start)/1.0f;
        } else if (type == TAP_B || type == SCR_B_UP || type == SCR_B_DOWN || type == SCR_B_ANY) {
            if(type== TAP_B) {
                sprite.setTextureRect(sf::IntRect(800,0,400,400));
            } else if(type == SCR_B_UP) {
                sprite.setTextureRect(sf::IntRect(800,840,400,400));
            } else if(type == SCR_B_DOWN) {
                sprite.setTextureRect(sf::IntRect(400,840,400,400));
            } else if(type == SCR_B_ANY) {
                sprite.setTextureRect(sf::IntRect(0,840,400,400));
            }
            if(v.at(i).getLanMod() <= 1){
               //blue center
                m_start = sf::Vector2f(554.0,200.0);
                m_end = sf::Vector2f(600.0,500.0);
            }else if(v.at(i).getLanMod()==2){
                //blue right
                m_start = sf::Vector2f(590.0,200.0);
                m_end = sf::Vector2f(688.0,500.0);
            }
            m_vel = (m_end-m_start)/1.0f;
        } else sprite.setTextureRect(sf::IntRect(0,0,0,0));
        sprite.setOrigin(200.0,200.0);

        if(dt >= -0.2 && dt <= 1.0) {
            //position and scale calculations
            sf::Vector2f pos = m_start + m_vel*(1.0f-dt);
            sf::Vector2f scl = m_scl_start +m_scl_vel*(1.0f-dt);

            //sprite drawn on screen
            if(v.at(i).getRender() == true) {
                sprite.setScale(scl);
                sprite.setPosition(pos);
                m_window.draw(sprite);
            }
        }

    }
}

void Rendr::lanes(float time, std::vector<Note>& ev) {
    sf::Vector2f green_center_start = sf::Vector2f(472.0,200.0);
    sf::Vector2f green_center_end = sf::Vector2f(426.0f,500.0f);
    sf::Vector2f green_left_start = sf::Vector2f(436.0,200.0);
    sf::Vector2f green_left_end = sf::Vector2f(338.0,500.0);

    sf::Vector2f blue_center_start = sf::Vector2f(554.0,200.0);
    sf::Vector2f blue_center_end = sf::Vector2f(600.0,500.0);
    sf::Vector2f blue_right_start = sf::Vector2f(590.0,200.0);
    sf::Vector2f blue_right_end = sf::Vector2f(688.0,500.0);

    sf::Vector2f green_center_vel = (green_center_end-green_center_start)/1.0f;
    sf::Vector2f green_left_vel = (green_left_end-green_left_start)/1.0f;

    sf::Vector2f blue_center_vel = (blue_center_end-blue_center_start)/1.0f;
    sf::Vector2f blue_right_vel = (blue_right_end-blue_right_start)/1.0f;

    std::vector<sf::Vertex> green_lane;
    std::vector<sf::Vertex> blue_lane;

    for(size_t i = 0; i < ev.size(); ++i){
        if(ev.at(i).getRender()){
            float dt = 1.0f-(ev.at(i).getMilli()-time);
            int s = dt*200;
            if(s >= 200)s = 199;

            if(ev.at(i).getType() == CROSS_L){
                for(int j = s;j >= 0; j--){
                    m_lanes.at(j) = 0;
                    m_ren_cross = 0;
                }
            }if(ev.at(i).getType() == CROSS_C){
                for(int j = s;j >= 0; j--){
                    m_lanes.at(j) = 1;
                    m_ren_cross = 1;
                }
            }
            if(ev.at(i).getType() == CROSS_R){
                for(int j = s;j >= 0; j--){
                    m_lanes.at(j) = 2;
                    m_ren_cross = 2;
                }
            }
        }
    }

    for(size_t i = 0 ; i < m_lanes.size(); ++i){
        float perc = (float)(i+1)/resolution;
        if(m_lanes.at(i)==0){
            sf::Vector2f left = green_left_start+green_left_vel*perc;
            sf::Vector2f right = blue_center_start+blue_center_vel*perc;

            sf::Vertex g(left);
            g.color = sf::Color::Green;
            sf::Vertex b(right);
            b.color = sf::Color::Blue;

            green_lane.push_back(g);
            blue_lane.push_back(b);
        }else if(m_lanes.at(i)==1){
            sf::Vector2f left = green_center_start+green_center_vel*perc;
            sf::Vector2f right = blue_center_start+blue_center_vel*perc;

            sf::Vertex g(left);
            g.color = sf::Color::Green;
            sf::Vertex b(right);
            b.color = sf::Color::Blue;

            green_lane.push_back(g);
            blue_lane.push_back(b);
        }else if(m_lanes.at(i)==2){
            sf::Vector2f left = green_center_start+green_center_vel*perc;
            sf::Vector2f right = blue_right_start+blue_right_vel*perc;

            sf::Vertex g(left);
            g.color = sf::Color::Green;
            sf::Vertex b(right);
            b.color = sf::Color::Blue;

            green_lane.push_back(g);
            blue_lane.push_back(b);
        }
    }

    sf::VertexArray red(sf::Lines, 2);
    red[0].position = sf::Vector2f(512.0,200.0);
    red[0].color = sf::Color::Red;
    red[1].position = sf::Vector2f(512.0,500.0);
    red[1].color = sf::Color::Red;

    m_window.draw(&green_lane[0],green_lane.size(),sf::LineStrip);
    m_window.draw(&blue_lane[0],blue_lane.size(),sf::LineStrip);
    m_window.draw(red);
}

void Rendr::events(float time,std::vector<Note>&ev) {
    for(size_t i = 0; i < ev.size(); ++i) {
        int type = ev.at(i).getType();
        float diff_start = ev.at(i).getMilli()-time;
        if(type == SCR_G_START) {
            float diff_end = -1;
            for(size_t j = 0; j < ev.size(); ++j) {
                if(ev.at(j).getType()==SCR_G_END) {
                    diff_end = ev.at(j).getMilli()-time;
                    break;
                }
            }
            sf::VertexArray varr(sf::Quads,4);
            for(size_t i = 0; i < varr.getVertexCount(); ++i)varr[i].color = sf::Color(57,172,64);

            sf::Vector2f start_l = sf::Vector2f(446.0,200.0);
            sf::Vector2f end_l = sf::Vector2f(386.0,500.0);
            sf::Vector2f start_r = sf::Vector2f(492,200.0);
            sf::Vector2f end_r = sf::Vector2f(466.0,500.0);

            if(m_ren_cross == 0){
                start_l = sf::Vector2f(410.0,200.0);
                end_l = sf::Vector2f(298.0,500.0);
                start_r = sf::Vector2f(456,200.0);
                end_r = sf::Vector2f(378.0,500.0);
            }

            sf::Vector2f vel_l = (end_l-start_l)/1.0f;
            sf::Vector2f vel_r = (end_r-start_r)/1.0f;

            if(diff_end != -1 && diff_end >= 0.0 && diff_start <= 1.0) {
                if(diff_start <= 1.0 && diff_start >= 0.0) {
                    varr[2].position = start_r+vel_r*(1.0f-diff_start);
                    varr[3].position = start_l+vel_l*(1.0f-diff_start);
                } else {
                    varr[2].position = end_r;
                    varr[3].position = end_l;
                }
                if(diff_end <= 1.0 && diff_end >= 0.0) {
                    varr[0].position = start_l+vel_l*(1.0f-diff_end);
                    varr[1].position = start_r+vel_r*(1.0f-diff_end);
                } else {
                    varr[0].position = start_l;
                    varr[1].position = start_r;
                }
            }

            m_window.draw(varr);
        } else if(type == SCR_B_START) {
            float diff_end = -1;
            for(size_t j = 0; j < ev.size(); ++j) {
                if(ev.at(j).getType()==SCR_B_END) {
                    diff_end = ev.at(j).getMilli()-time;
                    break;
                }
            }
            sf::VertexArray varr(sf::Quads,4);
            for(size_t i = 0; i < varr.getVertexCount(); ++i)varr[i].color = sf::Color(40,51,177);

            sf::Vector2f start_l = sf::Vector2f(526.0,200.0);

            sf::Vector2f end_l = sf::Vector2f(560.0,500.0);
            sf::Vector2f start_r = sf::Vector2f(578,200.0);
            sf::Vector2f end_r = sf::Vector2f(640.0,500.0);

            if (m_ren_cross == 2){
                start_l = sf::Vector2f(562.0,200.0);
                end_l = sf::Vector2f(648.0,500.0);
                start_r = sf::Vector2f(614,200.0);
                end_r = sf::Vector2f(728.0,500.0);
            }

            sf::Vector2f vel_l = (end_l-start_l)/1.0f;
            sf::Vector2f vel_r = (end_r-start_r)/1.0f;

            if(diff_end != -1 && diff_end >= 0.0 && diff_start <= 1.0) {
                if(diff_start <= 1.0 && diff_start >= 0.0) {
                    varr[2].position = start_r+vel_r*(1.0f-diff_start);
                    varr[3].position = start_l+vel_l*(1.0f-diff_start);
                } else {
                    varr[2].position = end_r;
                    varr[3].position = end_l;
                }
                if(diff_end <= 1.0 && diff_end >= 0.0) {
                    varr[0].position = start_l+vel_l*(1.0f-diff_end);
                    varr[1].position = start_r+vel_r*(1.0f-diff_end);
                } else {
                    varr[0].position = start_l;
                    varr[1].position = start_r;
                }
            }

            m_window.draw(varr);
        }
    }
}

void Rendr::pollState(float time,Player& p,Generator &g) {
    m_time_txt.setString("Time:"+std::to_string(time));
    m_score_txt.setString("Score:"+std::to_string(p.getScore()));
    m_combo_txt.setString("Combo:"+std::to_string(p.getCombo()));
    m_mult_txt.setString("Mult:"+std::to_string(p.getMult()));

    m_red = p.m_red;
    m_blue = p.m_blue;
    m_green = p.m_green;
    m_player_cross = p.m_cross;
}

Rendr::~Rendr() {
    //dtor
}
