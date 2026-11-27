#pragma once

#include "static_arr.hpp"
#include "avo_sensor.hpp"
#include "avo_peripherals.hpp"
#include "avo_self.hpp"


struct AvoSonar
{
	Servo      view_pivot;
	UltraSonic sonic_sensor;

	StaticArr<Vec2f, 3> point_cloud; // literally 1/4 of Uno RAM


	void init ()
	{
		view_pivot.set_pinmode();
		sonic_sensor.set_pinmode();
	}

	void update (AvoSelf& self)
	{
		auto dist = sonic_sensor.get_dist();
		if (dist == INFINITY) return;

		auto t_s = millis() * 0.00'1f;
		view_pivot.angle = int(360 * t_s / 2.0f) % 360;

		auto theta = DEG_TO_RAD * (view_pivot.angle + self.heading);
		auto echoing_point = self.pos + Vec2f::from_angle(theta, dist);

		if (point_cloud.count == point_cloud.capacity)
			point_cloud.shift_fwd_del_add(0, echoing_point);
		else
			point_cloud.push_back(echoing_point);

		print_map(0.5f);
	}

	void center_to (Vec2f pos) {
		for (u16 i = 0; i < point_cloud.count; i++)
			point_cloud[i] -= pos;
	}


	/** \param w_newl the width of print buffer including the line-ending newlines */
	template<size_t size>
	inline void populate_map_print_buf (char (&buf)[size], size_t w_newl, size_t h, float width_m)
	{
		if (size != (w_newl * h)) {
			Serial.println("Buffer size doesn't match width and height");
			return;
		}
		if ((w_newl < 2) or (h < 1)) {
			Serial.println("Buffer width or height isn't valid");
			return;
		}

		const size_t fac_x = w_newl - 2;
		const size_t fac_y = h - 1;


		memset(buf, ' ', size);

		for (uint16_t i = w_newl-1; i < size-1; i += w_newl)
			buf[i] = '\n';
		buf[size-1] = '\0';


		for (size_t i = 0; i < point_cloud.count; i++)
		{
			// add by 0.5 to center it
			const Vec2f point_pos = Vec2f(0.5, 0.5) + point_cloud[i] / width_m;

			const Vec2zu char_pos = {
				size_t( clamp<float>(roundf(fac_x * point_pos.x), 0, fac_x) ),
				size_t( clamp<float>(roundf(fac_y * point_pos.y), 0, fac_y) )
			};

			// invert y so that +y points up
			const size_t char_pos_y_up = fac_y - char_pos.y;

			const size_t char_idx = char_pos_y_up * w_newl + char_pos.x;
			buf[char_idx] = '#';
		}

		// mark center as S (self) if it's not overlapping a point, otherwise $
		const size_t center_idx = (h/2) * w_newl + (fac_x/2);
		buf[center_idx] = (' ' == buf[center_idx])? 'S' : '$';
	}

	/** \param width_m the total distance spanning the map in meters */
	void print_map (float width_m = 1.00f)
	{
		// the width of print buffer including the line-ending newlines
		constexpr size_t w_newl = 31 + 1;
		constexpr size_t h = 13;

		// yes, this is inefficient and wastes precious RAM
		static char print_buf[w_newl * h];
		populate_map_print_buf(print_buf, w_newl, h, width_m);

		Serial.print(F("-====== POINT CLOUD MAP ======-\n"));
		Serial.println(print_buf);
		Serial.print(F("-=============================-\n"));
	}
};
