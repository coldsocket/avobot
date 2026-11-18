#pragma once

#include "static_arr.hpp"
#include "avo_sensor.hpp"
#include "avo_peripherals.hpp"
#include "avo_self.hpp"


struct AvoSonar
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

		auto theta = view_pivot.angle + self.heading;
		auto echoing_point = self.pos + Vec2f::from_angle(theta, dist);

		if (point_cloud.count == point_cloud.capacity)
			point_cloud.shift_fwd_delete_add(0, echoing_point);
		else
			push_point(echoing_point);

		print_map();
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

	void print_map ()
	{
		constexpr size_t width_chr = 31;
		constexpr size_t height_chr = 13;
		// includes the ending newline
		constexpr size_t width_incl = width_chr + 1;

		// from 0 to width - 1
		constexpr size_t fac_x = width_chr - 1;
		constexpr size_t fac_y = height_chr - 1;

		constexpr float width_m = 1.00f;

		// 13(y) by (31 + 1)(x + '\n')
		// yes, this is inefficient and wastes precious RAM
		constexpr size_t buf_size = width_incl * height_chr;

		char print_buffer[buf_size];

		memset(print_buffer, ' ', buf_size);
		for (uint16_t i = width_incl-1; i < buf_size-1; i += width_incl)
			print_buffer[i] = '\n';
		print_buffer[buf_size-1] = '\0';

		// char print_buffer[buf_size] = {
		// 	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\n', // 1
		// 	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\n', // 2
		// 	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\n', // 3
		// 	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\n', // 4
		// 	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\n', // 5
		// 	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\n', // 6
		// 	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\n', // 7
		// 	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\n', // 8
		// 	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\n', // 9
		// 	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\n', // 10
		// 	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\n', // 11
		// 	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\n', // 12
		// 	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0', // 13
		// };


		for (size_t i = 0; i < point_cloud.count; i++)
		{
			// add by 0.5 to center it
			const Vec2f point_pos = Vec2f(0.5, 0.5) + point_cloud[i] / width_m;

			const Vec2T<size_t> char_pos = {
				clamp<size_t>(roundf(fac_x * point_pos.x), 0, fac_x),
				clamp<size_t>(roundf(fac_y * point_pos.y), 0, fac_y)
			};

			// invert y so that +y points up
			const size_t char_idx = (fac_y - char_pos.y) * width_incl + char_pos.x;
			print_buffer[char_idx] = '#';
		}

		// Self : Self-Overlapped
		print_buffer[6*32 + 15] =
			(' ' == print_buffer[6*32 + 15])?
			'S' : '$';


		Serial.print(F("-====== POINT CLOUD MAP ======-\n"));
		Serial.print(F("N = "));
		Serial.print(point_cloud.count);
		Serial.print(F(" | Newest: ("));
		Serial.print(point_cloud[point_cloud.count-1].x);
		Serial.print(F(", "));
		Serial.print(point_cloud[point_cloud.count-1].y);
		Serial.print(F(")\n"));
		Serial.print(F("============ 100cm ============\n"));
		Serial.print(print_buffer);
	}
};
