// Project: libv.re, File: src/libv/re/_ideas/task_graph.hpp

#pragma once


// With a task graph based async scheduler
//
//	for (const auto& stage : shader->stages())
// 		fork async([]{
// 			co_yield ThreadAffinity::FS;
//			loadStageFS(); // Compose source
// 			co_yield ThreadAffinity::GL;
//			loadStageGL(); // Compile shader stage
// 		}]);
//	joinStagesAsyncWork();
// 	co_yield ThreadAffinity::GL;
// 	loadStageGL(); // Link shader program and fetch uniform information
// ---
// Brainstorming
// auto s0 = fork(stage);
// auto s1 = fork(stage);
// auto s2 = fork(stage);
// co_await s0;
// co_await s1;
// co_await s2;
// co_yield ThreadAffinity::GL;
// loadGL
// ---
