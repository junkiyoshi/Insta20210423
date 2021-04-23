#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(0);
	ofNoFill();
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);
	
	this->font.loadFont("fonts/Kazesawa-Bold.ttf", 200, true, true, true);
	this->word = "A";
	int sample_count = 300;
	vector<ofPath> word_path = this->font.getStringAsPoints(word, true, false);
	for (int word_index = 0; word_index < word_path.size(); word_index++) {

		vector<ofPolyline> outline = word_path[word_index].getOutline();
		for (int outline_index = 0; outline_index < outline.size(); outline_index++) {

			outline[outline_index] = outline[outline_index].getResampledBySpacing(1);
			vector<glm::vec3> vertices = outline[outline_index].getVertices();
			for (int i = 0; i < vertices.size(); i++) {

				this->font_location_list.push_back(vertices[i] + glm::vec3(this->font.stringWidth(word) * -0.5, this->font.stringHeight(word) * 0.5, 0));
			}
		}
	}

	ofColor color;
	vector<int> hex_list = { 0x1f005c, 0x5b0060, 0x870160, 0xac255e, 0xca485c, 0xe16b5c, 0xf39060, 0xffb56b };
	for (auto hex : hex_list) {

		color.setHex(hex);
		this->base_color_list.push_back(color);
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	if (this->log_list.size() < 2500) {

		for (int i = 0; i < 60; i++) {

			int font_location_index = ofRandom(this->font_location_list.size());
			vector<glm::vec3> log;
			log.push_back(this->font_location_list[font_location_index]);
			this->log_list.push_back(log);
			this->speed_list.push_back(ofRandom(3, 10));
			this->color_list.push_back(this->base_color_list[(int)ofRandom(this->base_color_list.size())]);
		}
	}

	for (int i = this->log_list.size() - 1; i >= 0; i--) {

		if (glm::length(this->log_list[i].back()) > 360) {

			this->log_list.erase(this->log_list.begin() + i);
			this->speed_list.erase(this->speed_list.begin() + i);
			this->color_list.erase(this->color_list.begin() + i);

			continue;
		}

		auto radius = glm::length(this->log_list[i].back());
		auto next = glm::normalize(this->log_list[i].back()) * (radius + this->speed_list[i]);
		this->log_list[i].push_back(next);

		while (this->log_list[i].size() > 30) {

			this->log_list[i].erase(this->log_list[i].begin());
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(180);

	ofSetLineWidth(3);
	ofSetColor(this->base_color_list[this->base_color_list.size() - 1]);
	this->font.drawStringAsShapes(this->word, this->font.stringWidth(this->word) * -0.5, this->font.stringHeight(word) * 0.5);

	ofSetLineWidth(0.6);
	for (int i = 0; i < this->log_list.size(); i++) {

		auto alpha = 255.f;
		auto len = glm::length(this->log_list[i].back());
		if (len > 300) {
		
			alpha = ofMap(len, 300, 380, 255, 0);
		}
		
		ofSetColor(ofColor(this->color_list[i], alpha));

		ofBeginShape();
		ofVertices(this->log_list[i]);
		ofEndShape();
	}



	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}