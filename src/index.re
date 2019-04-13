open Reprocessing;

type fruitAsset = {
  imageHalf1: imageT,
  imageHalf2: imageT,
  image: imageT,
};

type fruit = {
  startPosition: (int, int),
  spawnFrame: int,
  fruitAsset,
};

type stateT = {
  background: imageT,
  fruitAssets: array(fruitAsset),
  visibleFruits: list(fruit),
};

let screenWidth = 600;
let screenHeight = 600;
let fruitSize = 100;
let speed = 3;

let setup = env => {
  Env.size(~width=screenWidth, ~height=screenHeight, env);

  let fruitAssets =
    [|"apple", "banana", "coconut", "orange", "pineapple", "watermelon"|]
    |> Array.map(name =>
         {
           imageHalf1:
             Draw.loadImage(
               ~filename="./assets/" ++ name ++ "_half_1.png",
               env,
             ),
           imageHalf2:
             Draw.loadImage(
               ~filename="./assets/" ++ name ++ "_half_2.png",
               env,
             ),
           image:
             Draw.loadImage(~filename="./assets/" ++ name ++ ".png", env),
         }
       );

  {
    background: Draw.loadImage(~filename="./assets/background.png", env),
    fruitAssets,
    visibleFruits: [],
  };
};

let draw = (state, env) => {
  /* print_endline("draw"); */
  /* Env.mouse for cutting the fruits */
  Draw.image(state.background, ~pos=(0, 0), env);

  let newState =
    if (Utils.random(~min=0, ~max=30) == 0) {
      let fruitIndex =
        Utils.random(~min=0, ~max=Array.length(state.fruitAssets) - 1);

      let fruitAsset = state.fruitAssets->(Array.unsafe_get(fruitIndex));

      let visibleFruits = [
        {
          startPosition: (
            Utils.random(~min=0, ~max=screenWidth),
            - fruitSize / 2,
          ),
          spawnFrame: Env.frameCount(env),
          fruitAsset,
        },
        ...state.visibleFruits,
      ];

      {...state, visibleFruits};
    } else {
      state;
    };

  newState.visibleFruits
  |> List.iter(({fruitAsset, startPosition: (startX, startY), spawnFrame}) =>
       Draw.image(
         fruitAsset.image,
         ~pos=(startX, (Env.frameCount(env) - spawnFrame + startY) * speed),
         ~width=fruitSize,
         ~height=fruitSize,
         env,
       )
     );

  newState;
};

run(~setup, ~draw, ());
