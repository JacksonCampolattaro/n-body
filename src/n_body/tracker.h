//
// Created by jackcamp on 2/6/19.
//

#ifndef N_BODY_TRACKER_H
#define N_BODY_TRACKER_H


class tracker {
public:
    static tracker* instance();
    static void recordFrame();

private:
    static tracker* internalInstance; // The tracker contains one and only one instance of itself

    tracker(tracker const&) = default; // The copy constructor is private
    tracker& operator=(tracker const&){}; // The assignment operator is private
    tracker() = default; // Only the tracker is allowed to construct itself


};


#endif //N_BODY_TRACKER_H
