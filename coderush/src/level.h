#ifndef CODERUSH_LEVEL_H_
#define CODERUSH_LEVEL_H_

#include <string>
#include <vector>
#include <utility>

namespace Coderush {
	class Level {
		public:
			enum class State {
				EMPTY,
				WALL,
			};
			using Map = std::vector<std::vector<State>>;
			using position = std::pair<int, int>;

			Level(const Map& map, position start_pos, position exit_pos)
					: map(map), cur_pos(start_pos), exit_pos(exit_pos) {
			}

			std::vector<State>& operator[](int i) {
				return map[i];
			}

			position current_position() const {
				return cur_pos;
			}

			position exit_position() const {
				return exit_pos;
			}

		private:
			Map map;
			position cur_pos;
			position exit_pos;
	};

	Level load_level(const std::string& filename);
}

#endif /* CODERUSH_LEVEL_H_ */
