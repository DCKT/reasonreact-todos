[@bs.val] external jsonParse : string => 'a = "JSON.parse";

module LocalStorage = {
  let namespace = "reasonreact-todos";
  let save: 'elements => unit =
    elements =>
      switch (Js.Json.stringifyAny(elements)) {
      | None => ()
      | Some(stringifiedElements) =>
        Dom.Storage.(localStorage |> setItem(namespace, stringifiedElements))
      };
  let getAll = () =>
    switch Dom.Storage.(localStorage |> getItem(namespace)) {
    | None => []
    | Some(elements) => jsonParse(elements)
    };
};