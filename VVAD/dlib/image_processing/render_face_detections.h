// Copyright (C) 2014  Davis E. King (davis@dlib.net)
// License: Boost Software License   See LICENSE.txt for the full license.
#ifndef DLIB_RENDER_FACE_DeTECTIONS_H_
#define DLIB_RENDER_FACE_DeTECTIONS_H_

#include "full_object_detection.h"
#include "../gui_widgets.h"
#include "render_face_detections_abstract.h"
#include <vector>


namespace dlib {
    inline std::vector<image_window::overlay_line> render_face_detections(
            const std::vector<full_object_detection> &dets,
            const rgb_pixel color = rgb_pixel(0, 255, 0)
    ) {
        std::vector<image_window::overlay_line> lines;
        for (unsigned long i = 0; i < dets.size(); ++i) {
//            DLIB_CASSERT(dets[i].num_parts() == 68 || dets[i].num_parts() == 5,
//                "\t std::vector<image_window::overlay_line> render_face_detections()"
//                << "\n\t You have to give either a 5 point or 68 point face landmarking output to this function. "
//                << "\n\t dets["<<i<<"].num_parts():  " << dets[i].num_parts() 
//            );

            const full_object_detection &d = dets[i];

            if (d.num_parts() == 6) {
//              0-1 je spodok, 0-2 je spodok dolava,
                lines.push_back(image_window::overlay_line(d.part(0), d.part(1), color));
                lines.push_back(image_window::overlay_line(d.part(0), d.part(2), color));
                lines.push_back(image_window::overlay_line(d.part(1), d.part(3), color));
                lines.push_back(image_window::overlay_line(d.part(4), d.part(5), color));
                lines.push_back(image_window::overlay_line(d.part(4), d.part(2), color));
                lines.push_back(image_window::overlay_line(d.part(5), d.part(3), color));
                continue;
            }

            if (d.num_parts() == 5) {
                lines.push_back(image_window::overlay_line(d.part(0), d.part(1), color));
                lines.push_back(image_window::overlay_line(d.part(1), d.part(4), color));
                lines.push_back(image_window::overlay_line(d.part(4), d.part(3), color));
                lines.push_back(image_window::overlay_line(d.part(3), d.part(2), color));
                continue;
            }

            if (d.num_parts() == 20) {
                // Lips outer part
                for (unsigned long i = 1; i <= 11; ++i)
                    lines.push_back(image_window::overlay_line(d.part(i), d.part(i - 1), color));
                lines.push_back(image_window::overlay_line(d.part(0), d.part(11), color));

                // Lips inside part
                for (unsigned long i = 13; i <= 19; ++i)
                    lines.push_back(image_window::overlay_line(d.part(i), d.part(i - 1), color));
                lines.push_back(image_window::overlay_line(d.part(12), d.part(19), color));
                continue;
            }

            if (d.num_parts() == 27) {
                // Lips outer part
                for (unsigned long i = 8; i <= 18; ++i)
                    lines.push_back(image_window::overlay_line(d.part(i), d.part(i - 1), color));
                lines.push_back(image_window::overlay_line(d.part(7), d.part(18), color));

                // Lips inside part
                for (unsigned long i = 20; i <= 26; ++i)
                    lines.push_back(image_window::overlay_line(d.part(i), d.part(i - 1), color));
                lines.push_back(image_window::overlay_line(d.part(19), d.part(26), color));

                lines.push_back(image_window::overlay_line(d.part(0), d.part(1), color));
                lines.push_back(image_window::overlay_line(d.part(1), d.part(2), color));
                lines.push_back(image_window::overlay_line(d.part(2), d.part(6), color));
                lines.push_back(image_window::overlay_line(d.part(6), d.part(3), color));
                lines.push_back(image_window::overlay_line(d.part(3), d.part(5), color));
                lines.push_back(image_window::overlay_line(d.part(5), d.part(0), color));
                lines.push_back(image_window::overlay_line(d.part(3), d.part(4), color));
                continue;
            }

            if (d.num_parts() == 68) {
                // Around Chin. Ear to Ear
                for (unsigned long i = 1; i <= 16; ++i)
                    lines.push_back(image_window::overlay_line(d.part(i), d.part(i - 1), color));

                // Line on top of nose
                for (unsigned long i = 28; i <= 30; ++i)
                    lines.push_back(image_window::overlay_line(d.part(i), d.part(i - 1), color));

                // left eyebrow
                for (unsigned long i = 18; i <= 21; ++i)
                    lines.push_back(image_window::overlay_line(d.part(i), d.part(i - 1), color));
                // Right eyebrow
                for (unsigned long i = 23; i <= 26; ++i)
                    lines.push_back(image_window::overlay_line(d.part(i), d.part(i - 1), color));
                // Bottom part of the nose
                for (unsigned long i = 31; i <= 35; ++i)
                    lines.push_back(image_window::overlay_line(d.part(i), d.part(i - 1), color));
                // Line from the nose to the bottom part above
                lines.push_back(image_window::overlay_line(d.part(30), d.part(35), color));

                // Left eye
                for (unsigned long i = 37; i <= 41; ++i)
                    lines.push_back(image_window::overlay_line(d.part(i), d.part(i - 1), color));
                lines.push_back(image_window::overlay_line(d.part(36), d.part(41), color));

                // Right eye
                for (unsigned long i = 43; i <= 47; ++i)
                    lines.push_back(image_window::overlay_line(d.part(i), d.part(i - 1), color));
                lines.push_back(image_window::overlay_line(d.part(42), d.part(47), color));

                // Lips outer part
                for (unsigned long i = 49; i <= 59; ++i)
                    lines.push_back(image_window::overlay_line(d.part(i), d.part(i - 1), color));
                lines.push_back(image_window::overlay_line(d.part(48), d.part(59), color));

                // Lips inside part
                for (unsigned long i = 61; i <= 67; ++i)
                    lines.push_back(image_window::overlay_line(d.part(i), d.part(i - 1), color));
                lines.push_back(image_window::overlay_line(d.part(60), d.part(67), color));

                lines.push_back(image_window::overlay_line(d.part(48), d.part(54), rgb_pixel(0, 0, 255)));
                lines.push_back(image_window::overlay_line(d.part(50), d.part(58), rgb_pixel(0, 0, 255)));
                lines.push_back(image_window::overlay_line(d.part(52), d.part(56), rgb_pixel(0, 0, 255)));
                continue;
            }

        }
        return lines;
    }

// ----------------------------------------------------------------------------------------

    inline std::vector<image_window::overlay_line> render_face_detections(
            const full_object_detection &det,
            const rgb_pixel color = rgb_pixel(0, 255, 0)
    ) {
        std::vector<full_object_detection> dets;
        dets.push_back(det);
        return render_face_detections(dets, color);
    }

// ----------------------------------------------------------------------------------------

}

#endif // DLIB_RENDER_FACE_DeTECTIONS_H_

