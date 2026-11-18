#pragma once

#include "avo-sensor.hpp"
#include "avo-peripherals.hpp"
#include "static_arr.hpp"
#include "avo-self.hpp"


struct AvoRadar
{
	Servo      view_pivot;
	UltraSonic sonic_sensor;

	StaticArr<Vec2f, 64> point_cloud; // literally 1/4 of Uno RAM


	void init ()
	{
		view_pivot.set_pinmode();
		sonic_sensor.set_pinmode();
	}

	void update (AvoSelf& self)
	{
		float dist = sonic_sensor.get_dist();
		if (dist == INFINITY) return;

		auto theta = view_pivot.angle;
		auto echoing_point = self.pos + Vec2f::from_angle(theta, dist);

		if (point_cloud.count == point_cloud.capacity)
			point_cloud.shift_front_delete_add(0, echoing_point);
		else
			push_point(echoing_point);

		print_repr();
	}


	bool push_point (Vec2f point) {
		return point_cloud.push_back(point);
	}

	/** \return -1 = not found */
	size_t pop_point (Vec2f point) {
		return point_cloud.pop(point);
	}

	void center_to (Vec2f pos) {
		for (u16 i = 0; i < point_cloud.count; i++)
			point_cloud[i] -= pos;
	}

	void print_repr ()
	{
		// 13y by 31x
		// yes, this is inefficient and wastes precious RAM
		char print_buffer[] = {
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\n', // 1
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\n', // 2
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\n', // 3
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\n', // 4
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\n', // 5
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\n', // 6
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\n', // 7
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\n', // 8
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\n', // 9
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\n', // 10
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\n', // 11
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\n', // 12
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0', // 13
		};

		for (size_t i = 0; i < point_cloud.count; i++)
		{
			Vec2f pos = point_cloud[i];
			pos = pos / 1.0f + Vec2f(0.5, 0.5);
			pos = { clamp<float>(pos.x, 0, 1), clamp<float>(pos.y, 0, 1) };

			Vec2T<size_t> char_pos = {
				(size_t)roundf(30 * pos.x),
				(size_t)roundf(12 * pos.y)
			};
			size_t char_idx = char_pos.y * 32 + char_pos.x;
			print_buffer[char_idx] = '#';
		}

		// Self
		print_buffer[6*32 + 16] = '@';

		Serial.print("-=== POINT CLOUD ASCII REPR ===-\n");
		Serial.print("N = ");
		Serial.print(point_cloud.count);
		Serial.print(" | Newest: (");
		Serial.print(point_cloud[point_cloud.count-1].x);
		Serial.print(", ");
		Serial.print(point_cloud[point_cloud.count-1].y);
		Serial.print(")\n");
		Serial.print("============ 100 cm ============\n");
		Serial.print(print_buffer);
		Serial.print("-========== END REPR ==========-\n");
	}
};
